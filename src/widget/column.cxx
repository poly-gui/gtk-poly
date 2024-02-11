//
// Created by kenym on 10/02/24.
//

#include "column.hxx"

#include "widget_factory.hxx"

Poly::Column::Column(const Message::Column &column)
	: Box(Gtk::Orientation::VERTICAL, 0),
	  tag(column.tag.has_value() ? *column.tag : 0) {
	switch (column.horizontal_alignment) {
	case Message::Alignment::START:
		horizontal_alignment = Gtk::Align::START;
		break;
	case Message::Alignment::END:
		horizontal_alignment = Gtk::Align::END;
		break;
	default:
		horizontal_alignment = Gtk::Align::CENTER;
		break;
	}

	switch (column.vertical_alignment) {
	case Message::Alignment::START:
		vertical_alignment = Gtk::Align::START;
		break;
	case Message::Alignment::END:
		vertical_alignment = Gtk::Align::END;
		break;
	default:
		vertical_alignment = Gtk::Align::CENTER;
		break;
	}
}

void Poly::Column::append(std::shared_ptr<Widget> widget) {
	widget->set_halign(horizontal_alignment);
	widget->set_valign(vertical_alignment);
	this->append(*widget);
	children.emplace_back(std::move(widget));
}

std::unique_ptr<Poly::Column>
Poly::make_column(const Message::Column &column,
				  std::shared_ptr<Application> app) {
	auto col = std::make_unique<Column>(column);
	for (const std::unique_ptr<Message::Widget> &child : column.children) {
		std::shared_ptr<Gtk::Widget> widget = make_widget(*child, app);
		col->append(std::move(widget));
	}
	return col;
}