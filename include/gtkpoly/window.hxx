#ifndef GTKPOLY_WINDOW_HXX
#define GTKPOLY_WINDOW_HXX

#include <gtkmm/widget.h>
#include <gtkmm/window.h>

namespace Poly {

class WindowManager {
  private:
	std::unordered_map<std::string, std::shared_ptr<Gtk::Window>> windows;

  public:
	size_t active_window_count() const;

	std::shared_ptr<Gtk::Window> new_window_with_tag(const std::string &tag);

	std::shared_ptr<Gtk::Window>
	find_window_with_tag(const std::string &tag) const;

	void remove_window_by_tag(const std::string &tag);
};

} // namespace Poly

#endif // GTKPOLY_WINDOW_HXX
