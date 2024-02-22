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

	signal_clicked().connect([this, app = std::move(app)] {
		const auto now = std::chrono::system_clock::now();
		const Message::ClickEvent click_event(
			std::chrono::duration_cast<std::chrono::seconds>(
				now.time_since_epoch())
			.count());
		const Message::InvokeCallback invoke_callback(on_click_handle,
			click_event);

		app->portable_layer().send_message(invoke_callback);
	});
}

std::unique_ptr<Poly::Button>
Poly::make_button(const Message::Button &button,
                  std::shared_ptr<Application> app) {
	auto btn = std::make_unique<Button>(button, std::move(app));
	return btn;
}

void Poly::update_button(Button &button, const Message::Button &new_config) {
	button.set_label(new_config.text);
	button.on_click_handle = new_config.on_click;
}