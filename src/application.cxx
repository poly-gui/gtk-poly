#include "messages/create_window.np.hxx"
#include "messages/widgets/create_widget.np.hxx"
#include "messages/widgets/update_widget.np.hxx"
#include "widget/widget_factory.hxx"
#include "widget/widget_updater.hxx"

#include <glibmm.h>
#include <gtkpoly/application.hxx>
#include <iostream>
#include <memory>
#include <nanopack/message.hxx>

Poly::Application::Application(Private, const ApplicationConfig &config)
	: Gtk::Application(config.application_id, config.flags), config(config),
	  _portable_layer(config.app_dir_path / "bundle") {
	_portable_layer.on_message(
		[this](std::unique_ptr<NanoPack::Message> message) {
			handle_message(std::move(message));
		});
}

std::shared_ptr<Poly::Application>
Poly::Application::create(const ApplicationConfig &config) {
	return std::make_shared<Application>(Private{}, config);
}

Poly::PortableLayer &Poly::Application::portable_layer() {
	return _portable_layer;
}

Poly::WidgetRegistry &Poly::Application::widget_registry() {
	return _widget_registry;
}

int Poly::Application::start() {
	_portable_layer.spawn();
	const int status = run();
	cleanup();
	return status;
}

void Poly::Application::on_activate() {
	// at this point, the portable layer might not have finished initializing.
	// because no default window is associated with the application unless the
	// portable layer request for one, it will quit immediately.
	//
	// therefore, we need the application to continue running
	// to wait for the portable layer to finish initializing and to request a
	// new window
	hold();
}

void Poly::Application::on_poly_window_destroyed(
	const std::string &window_tag) {
	window_manager.remove_window_by_tag(window_tag);
	if (window_manager.active_window_count() == 0) {
		// no window is active, quit the application
		cleanup();
		quit();
	}
}

void Poly::Application::cleanup() { _portable_layer.terminate(); }

void Poly::Application::handle_message(std::unique_ptr<NanoPack::Message> msg) {
	switch (msg->type_id()) {
	case Message::CreateWindow::TYPE_ID:
		create_window(std::move(msg));
		break;

	case Message::CreateWidget::TYPE_ID:
		create_widget(std::move(msg));
		break;

	case Message::UpdateWidget::TYPE_ID:
		update_widget(std::move(msg));
		break;

	default:
		break;
	}
}

void Poly::Application::create_window(std::unique_ptr<NanoPack::Message> msg) {
	const auto create_window_msg =
		static_cast<Message::CreateWindow *>(msg.get());

	const std::shared_ptr<Window> window =
		window_manager.new_window_with_tag(create_window_msg->tag);
	window->set_title(create_window_msg->title);
	window->set_default_size(create_window_msg->width,
	                         create_window_msg->height);

	window->signal_destroy().connect([this, tag = create_window_msg->tag] {
		on_poly_window_destroyed(tag);
	});

	Glib::signal_idle().connect_once([this, window] {
		add_window(*window);
		window->show();
	});
}

void Poly::Application::create_widget(std::unique_ptr<NanoPack::Message> msg) {
	const auto create_widget_msg =
		static_cast<Message::CreateWidget *>(msg.get());

	const std::shared_ptr<Window> window =
		window_manager.find_window_with_tag(create_widget_msg->window_tag);
	if (window == nullptr)
		return;

	std::shared_ptr widget =
		make_widget(create_widget_msg->get_widget(), shared_from_this());
	widget->show();
	window->set_child(std::move(widget));
}

void Poly::Application::update_widget(std::unique_ptr<NanoPack::Message> msg) {
	const auto update_widget_msg =
		static_cast<Message::UpdateWidget *>(msg.release());

	Glib::RefPtr<Gtk::Widget> widget =
		_widget_registry.find_widget(update_widget_msg->tag);
	if (widget == nullptr) {
#ifdef DEBUG
		std::cout << "[WARNING] requested to update widget with tag "
			<< update_widget_msg->tag << " but it doesn't exist." << std::endl;
#endif
		return;
	}

	Glib::signal_idle().connect_once(
		[widget = std::move(widget), msg = update_widget_msg] {
			Poly::update_widget(*widget, msg->get_widget(), msg->args);
			delete msg;
		});
}
