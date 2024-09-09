#include "widget_registry.hxx"
#include <cstdint>

void Poly::WidgetRegistry::register_widget(
	uint32_t tag, const Glib::RefPtr<Gtk::Widget> &widget) {
	widgets.insert({tag, widget});
}

void Poly::WidgetRegistry::unregister_widget(uint32_t tag) {
	widgets.erase(tag);
}

Glib::RefPtr<Gtk::Widget>
Poly::WidgetRegistry::find_widget(const uint32_t tag) {
	const auto entry = widgets.find(tag);
	if (entry == widgets.end()) {
		return nullptr;
	}
	return entry->second;
}
