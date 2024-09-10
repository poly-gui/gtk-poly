#include <chrono>
#include <memory>
#include <sigc++/connection.h>

#include "../application.hxx"
#include "../rpc/event/click_event.np.hxx"
#include "button.hxx"

Poly::Button::Button(const Rpc::Button &button,
					 std::shared_ptr<_Application> app)
	: Gtk::Button(button.text), tag(button.tag.has_value() ? *button.tag : -1),
	  on_click_handle(button.on_click) {
	set_halign(Gtk::Align::START);
	set_valign(Gtk::Align::START);

	signal_clicked().connect(
		[on_click_handle = this->on_click_handle, app = std::move(app)] {
			const auto now = std::chrono::system_clock::now();
			app->portable_layer().invoke_void_callback(
				on_click_handle,
				std::make_unique<Rpc::ClickEvent>(
					std::chrono::duration_cast<std::chrono::seconds>(
						now.time_since_epoch())
						.count()));
		});
}

Glib::RefPtr<Poly::Button>
Poly::Button::create(const Rpc::Button &button,
					 std::shared_ptr<_Application> app) {
	return Glib::make_refptr_for_instance<Button>(
		new Button(button, std::move(app)));
}

void Poly::Button::update(const Rpc::Button &msg) {
	set_label(msg.text);
	on_click_handle = msg.on_click;
}
