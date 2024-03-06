#include "row.hxx"
#include "spacer.hxx"

#include "../dimens.hxx"
#include "widget_factory.hxx"

#include <gtkmm/label.h>

Poly::Row::Row(const Message::Row &row, std::shared_ptr<Application> app)
	: Box(Gtk::Orientation::HORIZONTAL, 0),
	  tag(row.tag.has_value() ? *row.tag : 0) {
	const int desired_width = static_cast<int>(round(row.width));
	const int desired_height = static_cast<int>(round(row.height));

	Widget *start_spacer = nullptr;
	Widget *end_spacer = nullptr;
	switch (row.horizontal_alignment) {
	case Message::Alignment::CENTER:
		if (desired_width != Dimension::MIN_CONTENT) {
			start_spacer = Gtk::make_managed<HorizontalSpacer>();
			end_spacer = Gtk::make_managed<HorizontalSpacer>();
			append(*start_spacer);
			append(*end_spacer);
		}
		horizontal_alignment = Gtk::Align::CENTER;
		break;

	case Message::Alignment::END:
		if (desired_width != Dimension::MIN_CONTENT) {
			start_spacer = Gtk::make_managed<HorizontalSpacer>();
			append(*start_spacer);
		}
		horizontal_alignment = Gtk::Align::END;
		break;

	default:
		if (desired_width != Dimension::MIN_CONTENT) {
			end_spacer = Gtk::make_managed<HorizontalSpacer>();
			append(*end_spacer);
		}
		horizontal_alignment = Gtk::Align::START;
		break;
	}

	switch (row.vertical_alignment) {
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

	has_spacer = start_spacer != nullptr || end_spacer != nullptr;

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

	for (const std::unique_ptr<Message::Widget> &child : row.children) {
		Glib::RefPtr<Gtk::Widget> widget = make_widget(*child, app);
		append(std::move(widget));
	}
}

Glib::RefPtr<Poly::Row> Poly::Row::create(const Message::Row &msg,
										  std::shared_ptr<Application> app) {
	return Glib::make_refptr_for_instance<Row>(new Row(msg, std::move(app)));
}

void Poly::Row::append(std::shared_ptr<Widget> widget) {
	if (widget->get_halign() != Gtk::Align::FILL) {
		widget->set_halign(horizontal_alignment);
	}
	if (widget->get_valign() != Gtk::Align::FILL) {
		widget->set_valign(vertical_alignment);
	}
	if (has_spacer) {
		if (children.empty()) {
			Widget *start_spacer;
			switch (horizontal_alignment) {
			case Gtk::Align::END:
				append(*widget);
				break;
			case Gtk::Align::START:
				insert_child_at_start(*widget);
				break;
			case Gtk::Align::CENTER:
				start_spacer = get_first_child();
				insert_child_after(*widget, *start_spacer);
			default:
				break;
			}
		} else {
			insert_child_after(*widget, *children.back());
		}
	} else {
		append(*widget);
	}
	children.emplace_back(std::move(widget));
}
