#include "messages/create_window.np.hxx"
#include "messages/widgets/create_widget.np.hxx"
#include "widget/widget_factory.hxx"

#include <glibmm.h>
#include <gtkpoly/application.hxx>
#include <memory>
#include <nanopack/message.hxx>

Poly::Application::Application(const ApplicationConfig &config)
	: Gtk::Application(config.application_id, config.flags), config(config),
	  portable_layer(config.app_dir_path / "bundle") {
	portable_layer.on_message(
		[this](std::unique_ptr<NanoPack::Message> message) {
			handle_message(std::move(message));
		});
}

int Poly::Application::start() {
	portable_layer.spawn();
	const int status = run();
	cleanup();
	return status;
}

void Poly::Application::on_activate() { hold(); }

void Poly::Application::cleanup() {}

void Poly::Application::handle_message(std::unique_ptr<NanoPack::Message> msg) {
	switch (msg->type_id()) {
	case Message::CreateWindow::TYPE_ID:
		create_window(static_cast<Message::CreateWindow *>(msg.get()));
		break;

	case Message::CreateWidget::TYPE_ID:
		create_widget(static_cast<Message::CreateWidget *>(msg.get()));
		break;

	default:
		break;
	}
}

void Poly::Application::create_window(const Message::CreateWindow *msg) {
	const std::shared_ptr<Window> window =
		window_manager.new_window_with_tag(msg->tag);
	window->set_title(msg->title);
	window->set_default_size(msg->width, msg->height);

	Glib::signal_idle().connect_once([this, window] {
		add_window(*window);
		window->show();
	});
}

void Poly::Application::create_widget(Message::CreateWidget *msg) const {
	const std::shared_ptr<Window> window =
		window_manager.find_window_with_tag(msg->window_tag);
	if (window == nullptr)
		return;

	const std::shared_ptr widget = make_widget(msg->get_widget(), *this);
	window->set_child(*widget);
	widget->show();
}
