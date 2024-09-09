#ifndef GTKPOLY__WIDGET_REGISTRY_HXX
#define GTKPOLY__WIDGET_REGISTRY_HXX

#include <cstdint>
#include <gtkmm/widget.h>
#include <memory>
#include <unordered_map>

namespace Poly {

class WidgetRegistry final {
	std::unordered_map<uint32_t, Glib::RefPtr<Gtk::Widget>> widgets;

  public:
	void register_widget(uint32_t tag, const Glib::RefPtr<Gtk::Widget> &widget);

	void unregister_widget(uint32_t tag);

	Glib::RefPtr<Gtk::Widget> find_widget(uint32_t tag);
};

} // namespace Poly

#endif // GTKPOLY__WIDGET_REGISTRY_HXX
