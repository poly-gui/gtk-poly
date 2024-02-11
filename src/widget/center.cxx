#include "center.hxx"

#include "widget_factory.hxx"

std::shared_ptr<Gtk::Widget>
Poly::make_center(const Message::Center &center,
				  std::shared_ptr<Application> app) {
	auto widget = make_widget(center.get_child(), std::move(app));
	widget->set_halign(Gtk::Align::CENTER);
	widget->set_valign(Gtk::Align::CENTER);
	return widget;
}
