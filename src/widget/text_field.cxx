#include "text_field.hxx"

#include "../rpc/widget/text_field_changed_event.np.hxx"
#include <memory>

Poly::TextField::TextField(const Rpc::TextField &text_field,
						   std::shared_ptr<_Application> app)
	: tag(text_field.tag.has_value() ? *text_field.tag : -1),
	  on_value_changed_handle(text_field.on_value_changed) {
	if (text_field.placeholder.has_value()) {
		set_placeholder_text(*text_field.placeholder);
	}
	set_hexpand(true);
	set_text(text_field.value);

	signal_changed().connect([this, app = std::move(app)] {
		app->portable_layer().invoke_void_callback(
			on_value_changed_handle,
			std::make_unique<Rpc::TextFieldChangedEvent>(get_text()));
	});
}

Glib::RefPtr<Poly::TextField>
Poly::TextField::create(const Rpc::TextField &text_field,
						std::shared_ptr<_Application> app) {
	return Glib::make_refptr_for_instance(new TextField(text_field, app));
}

void Poly::TextField::update(const Rpc::TextField &msg) {
	set_placeholder_text(msg.placeholder.has_value() ? *msg.placeholder : "");
	set_text(msg.value);
	on_value_changed_handle = msg.on_value_changed;
}
