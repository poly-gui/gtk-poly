#include "button.hxx"
#include <chrono>

#include "../messages/invoke_callback.np.hxx"
#include "../messages/widgets/button/click_event.np.hxx"

std::unique_ptr<Poly::Button>
Poly::make_button(const Message::Button &button,
                  std::shared_ptr<Application> app) {
	auto btn = std::make_unique<Button>(button.text);

	btn->signal_clicked().connect([cb_handle = button.on_click,
		app = std::move(app)] {
		const auto now = std::chrono::system_clock::now();
		const Message::ClickEvent click_event(
			std::chrono::duration_cast<std::chrono::seconds>(
				now.time_since_epoch())
			.count());
		const Message::InvokeCallback invoke_callback(cb_handle, click_event);

		app->portable_layer().send_message(invoke_callback);
	});

	return btn;
}

void Poly::update_button(Button &button, const Message::Button &new_config) {
	button.set_label(new_config.text);
}