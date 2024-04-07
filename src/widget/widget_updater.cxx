//
// Created by kenym on 10/02/24.
//

#include "widget_updater.hxx"

#include "../messages/widgets/button/button.np.hxx"
#include "../messages/widgets/list_view/list_view_batch_operations.np.hxx"
#include "../messages/widgets/text.np.hxx"
#include "../messages/widgets/text_field/text_field.np.hxx"
#include "button.hxx"
#include "list_view.hxx"
#include "nanopack/any.hxx"
#include "text.hxx"
#include "text_field.hxx"

#include <iostream>

void Poly::update_widget(Gtk::Widget &old_widget, Message::Widget &new_config,
						 const std::optional<NanoPack::Any> &args) {
	switch (new_config.type_id()) {
	case Message::Text::TYPE_ID:
		static_cast<Text &>(old_widget)
			.update(static_cast<Message::Text &>(new_config));
		break;

	case Message::Button::TYPE_ID:
		static_cast<Button &>(old_widget)
			.update(static_cast<Message::Button &>(new_config));
		break;

	case Message::TextField::TYPE_ID:
		static_cast<TextField &>(old_widget)
			.update(static_cast<Message::TextField &>(new_config));
		break;

	case Message::ListView::TYPE_ID:
		if (args.has_value()) {
			int bytes_read;
			auto operations =
				Message::ListViewBatchOperations(args->as_reader(), bytes_read);
			static_cast<ListView &>(old_widget)
				.update(static_cast<Message::ListView &>(new_config),
						operations);
		}
		break;

	default:
#ifdef DEBUG
		std::cout << "WARNING: unsupported update operation for widget #"
				  << new_config.type_id();
#endif
		break;
	}
}
