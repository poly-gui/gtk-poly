#include "widget_factory.hxx"

#include "../messages/widgets/center.np.hxx"
#include "../messages/widgets/column.np.hxx"
#include "../messages/widgets/text.np.hxx"
#include "center.hxx"
#include "column.hxx"
#include "text.hxx"

std::unique_ptr<Gtk::Widget> Poly::make_widget(Message::Widget &widget,
											   const Application &app) {
	switch (widget.type_id()) {
	case Message::Text::TYPE_ID:
		return make_text(static_cast<Message::Text &>(widget));
	case Message::Column::TYPE_ID:
		return make_column(static_cast<Message::Column &>(widget), app);
	case Message::Center::TYPE_ID:
		return make_center(static_cast<Message::Center &>(widget), app);
	default:
		return nullptr;
	}
}
