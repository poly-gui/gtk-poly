#include "widget_factory.hxx"

#include "../messages/widgets/center.np.hxx"
#include "../messages/widgets/column.np.hxx"
#include "../messages/widgets/text.np.hxx"
#include "../messages/widgets/button/button.np.hxx"
#include "center.hxx"
#include "column.hxx"
#include "text.hxx"
#include "button.hxx"

std::shared_ptr<Gtk::Widget>
Poly::make_widget(Message::Widget &widget, std::shared_ptr<Application> app) {
	std::shared_ptr<Gtk::Widget> w;

	switch (widget.type_id()) {
	case Message::Text::TYPE_ID:
		w = make_text(static_cast<Message::Text &>(widget), app);
		break;

	case Message::Column::TYPE_ID:
		w = make_column(static_cast<Message::Column &>(widget), app);
		break;

	case Message::Center::TYPE_ID:
		w = make_center(static_cast<Message::Center &>(widget), app);
		break;

	case Message::Button::TYPE_ID:
		w = make_button(static_cast<Message::Button &>(widget), app);
		break;

	default:
		return nullptr;
	}

	if (widget.tag.has_value()) {
		app->widget_registry().register_widget(*widget.tag, w);
	}

	return w;
}