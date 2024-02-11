//
// Created by kenym on 07/02/24.
//

#ifndef GTKPOLY_WINDOW_HXX
#define GTKPOLY_WINDOW_HXX

#include <gtkmm/widget.h>
#include <gtkmm/window.h>

namespace Poly {

class Window : public Gtk::Window {
	std::shared_ptr<Widget> root_widget;

  public:
	using Gtk::Window::set_child;
	void set_child(std::shared_ptr<Widget> widget);
};

class WindowManager {
  private:
	std::unordered_map<std::string, std::shared_ptr<Window>> windows;

  public:
	size_t active_window_count() const;

	std::shared_ptr<Window> new_window_with_tag(const std::string &tag);

	std::shared_ptr<Window> find_window_with_tag(const std::string &tag) const;

	void remove_window_by_tag(const std::string &tag);
};

} // namespace Poly

#endif // GTKPOLY_WINDOW_HXX
