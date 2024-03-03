#include "text_field.hxx"

#include "../messages/invoke_callback.np.hxx"
#include "../messages/widgets/text_field/on_value_changed.np.hxx"

Poly::TextField::TextField(const Message::TextField &text_field,
                           std::shared_ptr<Application> app)
	: tag(text_field.tag.has_value() ? *text_field.tag : -1),
	  on_value_changed_handle(text_field.on_value_changed) {
	if (text_field.placeholder.has_value()) {
		set_placeholder_text(*text_field.placeholder);
	}
	set_text(text_field.value);

	signal_changed().connect([this, app = std::move(app)] {
		const Message::OnValueChanged on_value_changed(get_text());
		app->portable_layer().invoke_callback(on_value_changed_handle,
		                                      on_value_changed);
	});
}

std::unique_ptr<Poly::TextField>
Poly::make_text_field(const Message::TextField &text_field,
                      std::shared_ptr<Application> app) {
	return std::make_unique<TextField>(text_field, std::move(app));
}

void Poly::update_text_field(TextField &text_field,
                             const Message::TextField &new_config) {
	text_field.set_placeholder_text(
		new_config.placeholder.has_value() ? *new_config.placeholder : "");
	text_field.set_text(new_config.value);
	text_field.on_value_changed_handle = new_config.on_value_changed;
}