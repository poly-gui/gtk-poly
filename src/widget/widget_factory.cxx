#include <iostream>

#include "widget_factory.hxx"

#include "../rpc/widget/button.np.hxx"
#include "../rpc/widget/center.np.hxx"
#include "../rpc/widget/column.np.hxx"
#include "../rpc/widget/list_view.np.hxx"
#include "../rpc/widget/row.np.hxx"
#include "../rpc/widget/text.np.hxx"
#include "../rpc/widget/text_field.np.hxx"
#include "button.hxx"
#include "center.hxx"
#include "column.hxx"
#include "list_view.hxx"
#include "row.hxx"
#include "text.hxx"
#include "text_field.hxx"

Glib::RefPtr<Gtk::Widget> Poly::make_widget(Rpc::Widget &widget,
											std::shared_ptr<_Application> app) {
	Glib::RefPtr<Gtk::Widget> w;

	switch (widget.type_id()) {
	case Rpc::Text::TYPE_ID:
		w = Text::create(static_cast<Rpc::Text &>(widget));
		break;

	case Rpc::Row::TYPE_ID:
		w = Row::create(static_cast<Rpc::Row &>(widget), app);
		break;

	case Rpc::Column::TYPE_ID:
		w = Column::create(static_cast<Rpc::Column &>(widget), app);
		break;

	case Rpc::Center::TYPE_ID:
		w = make_center(static_cast<Rpc::Center &>(widget), app);
		break;

	case Rpc::Button::TYPE_ID:
		w = Button::create(static_cast<Rpc::Button &>(widget), app);
		break;

	case Rpc::TextField::TYPE_ID:
		w = TextField::create(static_cast<Rpc::TextField &>(widget), app);
		break;

	case Rpc::ListView::TYPE_ID:
		w = ListView::create(static_cast<Rpc::ListView &>(widget), app);
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
