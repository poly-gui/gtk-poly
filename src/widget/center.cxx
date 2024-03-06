#include "center.hxx"

#include "widget_factory.hxx"

#include <iostream>

Glib::RefPtr<Gtk::Widget> Poly::make_center(const Message::Center &center,
											std::shared_ptr<Application> app) {
	auto widget = make_widget(center.get_child(), std::move(app));
	if (widget->get_halign() != Gtk::Align::FILL) {
		widget->set_halign(Gtk::Align::CENTER);
	}
	if (widget->get_valign() != Gtk::Align::FILL) {
		widget->set_valign(Gtk::Align::CENTER);
	}
	return widget;
}