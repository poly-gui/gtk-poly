//
// Created by kenym on 07/02/24.
//


#ifndef GTKPOLY_WINDOW_HXX
#define GTKPOLY_WINDOW_HXX

#include <gtkmm/window.h>

namespace Poly {

class Window : public Gtk::Window {};

class WindowManager {
private:
	std::unordered_map<std::string, std::shared_ptr<Window>> windows;

public:
	std::shared_ptr<Window> new_window_with_tag(const std::string &tag);

	std::shared_ptr<Window> find_window_with_tag(const std::string &tag) const;
};

}

#endif //GTKPOLY_WINDOW_HXX
