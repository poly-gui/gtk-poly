#include <iostream>

#include "widget_factory.hxx"

#include "../messages/widgets/button/button.np.hxx"
#include "../messages/widgets/center.np.hxx"
#include "../messages/widgets/column.np.hxx"
#include "../messages/widgets/list_view/list_view.np.hxx"
#include "../messages/widgets/row.np.hxx"
#include "../messages/widgets/text.np.hxx"
#include "../messages/widgets/text_field/text_field.np.hxx"
#include "button.hxx"
#include "center.hxx"
#include "column.hxx"
#include "list_view.hxx"
#include "row.hxx"
#include "text.hxx"
#include "text_field.hxx"

Glib::RefPtr<Gtk::Widget> Poly::make_widget(Message::Widget &widget,
											std::shared_ptr<Application> app) {
	Glib::RefPtr<Gtk::Widget> w;

	switch (widget.type_id()) {
	case Message::Text::TYPE_ID:
		w = Text::create(static_cast<Message::Text &>(widget));
		break;

	case Message::Row::TYPE_ID:
		w = Row::create(static_cast<Message::Row &>(widget), app);
		break;

	case Message::Column::TYPE_ID:
		w = Column::create(static_cast<Message::Column &>(widget), app);
		break;

	case Message::Center::TYPE_ID:
		w = make_center(static_cast<Message::Center &>(widget), app);
		break;

	case Message::Button::TYPE_ID:
		w = Button::create(static_cast<Message::Button &>(widget), app);
		break;

	case Message::TextField::TYPE_ID:
		w = TextField::create(static_cast<Message::TextField &>(widget),
									app);
		break;

	case Message::ListView::TYPE_ID:
		w = ListView::create(static_cast<Message::ListView &>(widget),
								   app);
		break;

	default:
#ifdef DEBUG
		std::cout << "WARNING: unsupported widget type, type ID: "
				  << widget.type_id();
#endif
		return nullptr;
	}

	if (widget.tag.has_value()) {
		app->widget_registry().register_widget(*widget.tag, w);
	}

	return w;
}