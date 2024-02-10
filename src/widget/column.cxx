//
// Created by kenym on 10/02/24.
//

#include "column.hxx"

#include "widget_factory.hxx"

Poly::Column::Column(const Message::Column &column)
	: Box(Gtk::Orientation::VERTICAL, 0) {
	Gtk::Align halign;
	switch (column.horizontal_alignment) {
	case Message::Alignment::START:
		halign = Gtk::Align::START;
		break;
	case Message::Alignment::END:
		halign = Gtk::Align::END;
		break;
	default:
		halign = Gtk::Align::CENTER;
		break;
	}

	Gtk::Align valign;
	switch (column.vertical_alignment) {
	case Message::Alignment::START:
		valign = Gtk::Align::START;
		break;
	case Message::Alignment::END:
		valign = Gtk::Align::END;
		break;
	default:
		valign = Gtk::Align::CENTER;
		break;
	}

	set_halign(halign);
	set_valign(valign);
}

void Poly::Column::append(std::unique_ptr<Widget> widget) {
	this->append(*widget);
	children.emplace_back(std::move(widget));
}

std::unique_ptr<Poly::Column> Poly::make_column(const Message::Column &column,
												const Application &app) {
	auto col = std::make_unique<Column>(column);
	for (const std::unique_ptr<Message::Widget> &child : column.children) {
		std::unique_ptr<Gtk::Widget> widget = make_widget(*child, app);
		col->append(std::move(widget));
	}
	return col;
}
