#include "column.hxx"

#include "../dimens.hxx"
#include "widget_factory.hxx"

Poly::Column::Column(const Rpc::Column &column,
					 std::shared_ptr<_Application> app)
	: Box(Gtk::Orientation::VERTICAL, 0),
	  tag(column.tag.has_value() ? *column.tag : 0) {
	const int desired_width = static_cast<int>(round(column.width));
	const int desired_height = static_cast<int>(round(column.height));

	switch (column.horizontal_alignment) {
	case Rpc::Alignment::START:
		horizontal_alignment = Gtk::Align::START;
		break;
	case Rpc::Alignment::END:
		horizontal_alignment = Gtk::Align::END;
		break;
	default:
		horizontal_alignment = Gtk::Align::CENTER;
		break;
	}

	bool is_size_request_required = false;

	switch (desired_width) {
	case Dimension::FILL_PARENT:
		set_halign(Gtk::Align::FILL);
		set_hexpand(true);
		break;
	case Dimension::MIN_CONTENT:
		set_halign(Gtk::Align::START);
		set_hexpand(false);
		break;
	default:
		is_size_request_required = true;
		break;
	}

	switch (desired_height) {
	case Dimension::FILL_PARENT:
		set_valign(Gtk::Align::FILL);
		set_vexpand(true);
		break;
	case Dimension::MIN_CONTENT:
		set_valign(Gtk::Align::START);
		set_vexpand(false);
		break;
	default:
		is_size_request_required = true;
		break;
	}

	if (is_size_request_required) {
		set_size_request(desired_width, desired_height);
	}

	for (const std::unique_ptr<Rpc::Widget> &child : column.children) {
		Glib::RefPtr<Widget> widget = make_widget(*child, app);
		append(widget);
	}
}

Glib::RefPtr<Poly::Column>
Poly::Column::create(const Rpc::Column &msg,
					 std::shared_ptr<_Application> app) {
	return Glib::make_refptr_for_instance<Column>(
		new Column(msg, std::move(app)));
}

void Poly::Column::append(const Glib::RefPtr<Widget> &widget) {
	if (widget->get_halign() != Gtk::Align::FILL) {
		widget->set_halign(horizontal_alignment);
	}
	if (widget->get_valign() != Gtk::Align::FILL) {
		widget->set_valign(vertical_alignment);
	}
	append(*widget);
}
