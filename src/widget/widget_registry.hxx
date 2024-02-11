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
	std::unordered_map<int32_t, std::weak_ptr<Gtk::Widget>> widgets;

  public:
	void register_widget(int32_t tag, std::shared_ptr<Gtk::Widget> widget);

	std::shared_ptr<Gtk::Widget> find_widget(int32_t tag);
};

} // namespace Poly

#endif // GTKPOLY_WIDGET_REGISTRY_HXX
