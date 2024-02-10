//
// Created by kenym on 10/02/24.
//

#include "center.hxx"

#include "widget_factory.hxx"

std::unique_ptr<Gtk::Widget> Poly::make_center(const Message::Center &center,
											   const Application &app) {
	auto widget = make_widget(center.get_child(), app);
	widget->set_halign(Gtk::Align::CENTER);
	widget->set_valign(Gtk::Align::CENTER);
	return widget;
}
