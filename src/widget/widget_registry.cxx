//
// Created by kenym on 10/02/24.
//

#include "widget_registry.hxx"

void Poly::WidgetRegistry::register_widget(
	int32_t tag, const Glib::RefPtr<Gtk::Widget>& widget) {
	widgets.insert({tag, widget});
}

Glib::RefPtr<Gtk::Widget> Poly::WidgetRegistry::find_widget(const int32_t tag) {
	const auto entry = widgets.find(tag);
	if (entry == widgets.end()) {
		return nullptr;
	}
	return entry->second;
}
