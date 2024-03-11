//
// Created by kenym on 10/02/24.
//

#ifndef GTKPOLY_WIDGET_REGISTRY_HXX
#define GTKPOLY_WIDGET_REGISTRY_HXX

#include <cstdint>
#include <gtkmm/widget.h>
#include <memory>
#include <unordered_map>

namespace Poly {

class WidgetRegistry final {
	std::unordered_map<int32_t, Glib::RefPtr<Gtk::Widget>> widgets;

  public:
	void register_widget(int32_t tag, const Glib::RefPtr<Gtk::Widget> &widget);

	Glib::RefPtr<Gtk::Widget> find_widget(int32_t tag);
};

} // namespace Poly

#endif // GTKPOLY_WIDGET_REGISTRY_HXX
