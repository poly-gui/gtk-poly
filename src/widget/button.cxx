#include <chrono>
#include <optional>
#include <sigc++/connection.h>

#include "../messages/invoke_callback.np.hxx"
#include "../messages/widgets/button/click_event.np.hxx"
#include "button.hxx"

Poly::Button::Button(const Message::Button &button,
					 std::shared_ptr<Application> app)
	: Gtk::Button(button.text), tag(button.tag.has_value() ? *button.tag : -1),
	  on_click_handle(button.on_click) {
	set_halign(Gtk::Align::START);
	set_valign(Gtk::Align::START);

	signal_clicked().connect(
		[on_click_handle = this->on_click_handle, app = std::move(app)] {
			const auto now = std::chrono::system_clock::now();
			const Message::ClickEvent click_event(
				std::chrono::duration_cast<std::chrono::seconds>(
					now.time_since_epoch())
					.count());

			app->portable_layer().invoke_callback(on_click_handle, click_event);
		});
}

Glib::RefPtr<Poly::Button>
Poly::Button::create(const Message::Button &button,
					 std::shared_ptr<Application> app) {
	return Glib::make_refptr_for_instance<Button>(
		new Button(button, std::move(app)));
}

void Poly::Button::update(const Message::Button &msg) {
	set_label(msg.text);
	on_click_handle = msg.on_click;
}
