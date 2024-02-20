//
// Created by kenym on 10/02/24.
//

#include "widget_updater.hxx"

#include "../messages/widgets/button/button.np.hxx"
#include "../messages/widgets/text.np.hxx"
#include "../messages/widgets/text_field/text_field.np.hxx"
#include "button.hxx"
#include "text.hxx"
#include "text_field.hxx"

#include <iostream>

void Poly::update_widget(Gtk::Widget &old_widget, Message::Widget &new_config) {
	switch (new_config.type_id()) {
	case Message::Text::TYPE_ID:
		update_text(static_cast<Text &>(old_widget),
					static_cast<Message::Text &>(new_config));
		break;

	case Message::Button::TYPE_ID:
		update_button(static_cast<Button &>(old_widget),
					  static_cast<Message::Button &>(new_config));
		break;

	case Message::TextField::TYPE_ID:
		update_text_field(static_cast<TextField &>(old_widget),
						  static_cast<Message::TextField &>(new_config));
		break;

	default:
#ifdef DEBUG
		std::cout << "WARNING: unsupported update operation for widget #"
				  << new_config.type_id();
#endif
		break;
	}
}