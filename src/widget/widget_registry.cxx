//
// Created by kenym on 10/02/24.
//

#include "widget_registry.hxx"

void Poly::WidgetRegistry::register_widget(
	int32_t tag, std::shared_ptr<Gtk::Widget> widget) {
	widgets.insert({tag, widget});
}

std::shared_ptr<Gtk::Widget> Poly::WidgetRegistry::find_widget(int32_t tag) {
	const auto entry = widgets.find(tag);
	if (entry == widgets.end()) {
		return nullptr;
	}

	const std::weak_ptr<Gtk::Widget> widget_wp = entry->second;
	if (auto widget = widget_wp.lock()) {
		return widget;
	}

	widgets.erase(entry);

	return nullptr;
}
