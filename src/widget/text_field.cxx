#include "text_field.hxx"

#include "../messages/widgets/text_field/on_value_changed.np.hxx"

Poly::TextField::TextField(const Message::TextField &text_field,
						   std::shared_ptr<Application> app)
	: tag(text_field.tag.has_value() ? *text_field.tag : -1),
	  on_value_changed_handle(text_field.on_value_changed) {
	if (text_field.placeholder.has_value()) {
		set_placeholder_text(*text_field.placeholder);
	}
	set_vexpand(true);
	set_text(text_field.value);

	signal_changed().connect([this, app = std::move(app)] {
		const Message::OnValueChanged on_value_changed(get_text());
		app->portable_layer().invoke_callback(on_value_changed_handle,
											  on_value_changed);
	});
}

Glib::RefPtr<Poly::TextField>
Poly::TextField::create(const Message::TextField &text_field,
						std::shared_ptr<Application> app) {
	return Glib::make_refptr_for_instance(new TextField(text_field, app));
}

void Poly::TextField::update(const Message::TextField &msg) {
	set_placeholder_text(msg.placeholder.has_value() ? *msg.placeholder : "");
	set_text(msg.value);
	on_value_changed_handle = msg.on_value_changed;
}
