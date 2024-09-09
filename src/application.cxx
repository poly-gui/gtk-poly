#include "glibmm/main.h"
#include "glibmm/refptr.h"
#include "gtkmm/widget.h"
#include "gtkmm/window.h"
#include "nanopack/rpc.hxx"
#include "rpc/native_layer_service.np.hxx"
#include "rpc/portable_layer_service.np.hxx"
#include "widget/widget.hxx"
#include "widget/widget_factory.hxx"
#include "widget/widget_updater.hxx"
#include <cstddef>
#include <filesystem>
#include <glibmm.h>
#include <gtkmm/label.h>
#include <gtkpoly/application.hxx>
#include <iostream>
#include <memory>
#include <mutex>
#include <nanopack/message.hxx>
#include <optional>

#include "application.hxx"

std::shared_ptr<Poly::Application>
Poly::Application::create(const ApplicationConfig &config) {
	return std::make_shared<Poly::_Application>(config);
}

Poly::_Application::_Application(const ApplicationConfig &config)
	: Poly::Application(config.application_id, config.flags),
	  NativeLayerServiceServer(), config(config), _portable_layer() {}

Rpc::PortableLayerServiceClient &Poly::_Application::portable_layer() {
	return _portable_layer;
}

Poly::WidgetRegistry &Poly::_Application::widget_registry() {
	return _widget_registry;
}

int Poly::_Application::start() {
	spawn_portable_layer();
	const int status = run();
	cleanup();
	return status;
}

void Poly::_Application::spawn_portable_layer() {
	constexpr int READ_FD = 0;
	constexpr int WRITE_FD = 1;

	std::filesystem::path bin_path = config.app_dir_path / "bundle";

	// receive message from portable layer through this pipe
	int child_stdout_pipe[2];
	// send message to portable layer through this pipe
	int child_stdin_pipe[2];

	pipe(child_stdin_pipe);
	pipe(child_stdout_pipe);

	const int pid = fork();
	if (const bool is_child = pid == 0; is_child) {
		// file descriptor of stdout of native layer
		// messages from native layer are sent to here which can be read with
		// the read descriptor (READ_FD)
		const int parent_stdout = child_stdin_pipe[READ_FD];
		// file descriptor of stdin of parent process
		// portable layer will send messages through here by *writing* from here
		// with the write descriptor (WRITE_FD)
		const int parent_stdin = child_stdout_pipe[WRITE_FD];

		//                   (1)
		//          stdout ------> stdin
		//        /                      \
		//  native                        portable
		//        \                      /
		//          stdin  <------ stdout
		//                   (2)

		// pipe stdout of native layer to stdin of poratble layer
		// (1)
		dup2(parent_stdout, STDIN_FILENO);
		// pipe stdout of portable layer to stdin of native layer
		// (2)
		dup2(parent_stdin, STDOUT_FILENO);

		close(child_stdin_pipe[WRITE_FD]);
		close(child_stdout_pipe[READ_FD]);

		const char *bin_path_c_str = bin_path.c_str();
		execl(bin_path_c_str, bin_path_c_str, nullptr);

		std::cout << "should not be here: " << errno << std::endl;

		// TODO: handle when portable layer exits early here.
	} else {
		portable_layer_pid = pid;
		portable_layer_stdin_handle = child_stdin_pipe[WRITE_FD];
		portable_layer_stdout_handle = child_stdout_pipe[READ_FD];

		close(child_stdin_pipe[READ_FD]);
		close(child_stdout_pipe[WRITE_FD]);

		this->rpc_channel.emplace(portable_layer_stdin_handle,
								  portable_layer_stdout_handle);

		_portable_layer.use_channel(*rpc_channel);
		use_channel(*rpc_channel);

		rpc_channel->open();
	}
}

void Poly::_Application::on_activate() {
	// at this point, the portable layer might not have finished initializing.
	// because no default window is associated with the application unless the
	// portable layer request for one, it will quit immediately.
	//
	// therefore, we need the application to continue running
	// to wait for the portable layer to finish initializing and to request a
	// new window
	hold();
}

void Poly::_Application::on_poly_window_destroyed(
	const std::string &window_tag) {
	window_manager.remove_window_by_tag(window_tag);
	if (window_manager.active_window_count() == 0) {
		// no window is active, quit the application
		cleanup();
		quit();
	}
}

void Poly::_Application::cleanup() {
	if (portable_layer_pid > 0) {
		kill(portable_layer_pid, SIGKILL);
		close(portable_layer_stdin_handle);
		close(portable_layer_stdout_handle);
		portable_layer_pid = -1;
	}
}

void Poly::_Application::create_window(const std::string &title,
									   const std::string &description,
									   int32_t width, int32_t height,
									   const std::string &tag) {
	std::shared_ptr<Gtk::Window> window =
		window_manager.new_window_with_tag(tag);
	window->set_title(title);
	window->set_default_size(width, height);

	window->signal_destroy().connect(
		[this, tag] { on_poly_window_destroyed(tag); });

	Glib::signal_idle().connect_once([this, window = std::move(window),
									  tag = tag, title = title, width, height] {
		std::lock_guard<std::mutex> lock_guard(ui_mutex);
		add_window(*window);
		window->show();
	});
}

void Poly::_Application::clear_window(const std::string &window_tag) {
	const auto window = window_manager.find_window_with_tag(window_tag);
	if (window == nullptr) {
		return;
	}

	Glib::signal_idle().connect_once([this, window = std::move(window)] {
		std::lock_guard<std::mutex> lock_guard(ui_mutex);
		window->unset_child();
	});
}

void Poly::_Application::create_widget(std::unique_ptr<Rpc::Widget> widget,
									   const std::string &window_tag) {
	const std::shared_ptr<Gtk::Window> window =
		window_manager.find_window_with_tag(window_tag);
	if (window == nullptr) {
		return;
	}

	auto created_widget = make_widget(*widget, shared_from_this());

	Glib::signal_idle().connect_once([this, window, created_widget] {
		std::lock_guard<std::mutex> lock_guard(ui_mutex);
		window->set_child(*created_widget);
	});
}

void Poly::_Application::append_new_widget(std::unique_ptr<Rpc::Widget> child,
										   uint32_t parent_tag) {
	Glib::RefPtr<Gtk::Widget> found_widget =
		_widget_registry.find_widget(parent_tag);
	if (found_widget == nullptr) {
		return;
	}

	Glib::RefPtr<MultiChildrenWidget> mc_widget =
		std::dynamic_pointer_cast<MultiChildrenWidget>(found_widget);
	if (mc_widget == nullptr) {
		return;
	}

	Glib::RefPtr<Gtk::Widget> created_widget =
		make_widget(*child, shared_from_this());
	if (created_widget == nullptr) {
		return;
	}

	Glib::signal_idle().connect_once([this, mc_widget, created_widget] {
		std::lock_guard<std::mutex> lock_guard(ui_mutex);
		mc_widget->append_widget(created_widget);
	});
}

void Poly::_Application::insert_widget_before(
	std::unique_ptr<Rpc::Widget> widget,
	std::unique_ptr<Rpc::Widget> before_widget, uint32_t parent_tag) {
	Glib::RefPtr<Gtk::Widget> found_parent_widget =
		_widget_registry.find_widget(parent_tag);
	if (found_parent_widget == nullptr) {
		return;
	}
	Glib::RefPtr<Gtk::Widget> found_before_widget =
		_widget_registry.find_widget(*before_widget->tag);
	if (found_before_widget == nullptr) {
		return;
	}

	Glib::RefPtr<MultiChildrenWidget> parent_widget =
		std::dynamic_pointer_cast<MultiChildrenWidget>(found_parent_widget);
	if (parent_widget == nullptr) {
		return;
	}

	Glib::RefPtr<Gtk::Widget> created_widget =
		make_widget(*widget, shared_from_this());
	if (created_widget == nullptr) {
		return;
	}

	Glib::signal_idle().connect_once(
		[this, parent_widget, created_widget, found_before_widget] {
			std::lock_guard<std::mutex> lock_guard(ui_mutex);
			parent_widget->insert_widget_before(created_widget,
												found_before_widget);
		});
}

void Poly::_Application::update_widget(
	uint32_t tag, std::unique_ptr<Rpc::Widget> widget,
	std::unique_ptr<NanoPack::Message> args) {
	Glib::RefPtr<Gtk::Widget> found_widget = _widget_registry.find_widget(tag);
	if (found_widget == nullptr) {
#ifdef DEBUG
		std::cerr << "[WARNING] requested to update widget with tag " << tag
				  << " but it doesn't exist." << std::endl;
#endif
		return;
	}

	Glib::signal_idle().connect_once([this, widget = widget.release(),
									  args = args.release(), &found_widget] {
		std::lock_guard<std::mutex> lock_guard(ui_mutex);
		Poly::update_widget(*found_widget, *widget, args);
		delete args;
		delete widget;
	});
}

void Poly::_Application::update_widgets(
	const std::vector<uint32_t> &tag,
	const std::vector<std::unique_ptr<Rpc::Widget>> &widgets,
	std::unique_ptr<NanoPack::Message> args) {
	std::vector<Glib::RefPtr<Gtk::Widget>> found_widgets;
	found_widgets.reserve(widgets.size());

	for (const auto &_tag : tag) {
		Glib::RefPtr<Gtk::Widget> found_widget =
			_widget_registry.find_widget(_tag);
		if (found_widget == nullptr) {
			return;
		}
		found_widgets.emplace_back(found_widget);
	}

	Glib::signal_idle().connect_once([&] {
		std::lock_guard<std::mutex> lock_guard(ui_mutex);
		const size_t len = found_widgets.size();
		for (size_t i = 0; i < len; ++i) {
			// TODO: the args param should be a vector of messages rather than
			// just one arg because each update operation may have a separate
			// arg
			Poly::update_widget(*found_widgets[i], *widgets[i], nullptr);
		}
	});
}

void Poly::_Application::remove_widget(uint32_t tag) {
	Glib::RefPtr<Gtk::Widget> found_widget = _widget_registry.find_widget(tag);
	if (found_widget == nullptr) {
#ifdef DEBUG
		std::cerr << "[WARNING] requested to update widget with tag " << tag
				  << " but it doesn't exist." << std::endl;
#endif
		return;
	}

	Glib::signal_idle().connect_once([this, found_widget, tag] {
		std::lock_guard<std::mutex> lock_guard(ui_mutex);
		found_widget->unparent();
		_widget_registry.unregister_widget(tag);
	});
}
