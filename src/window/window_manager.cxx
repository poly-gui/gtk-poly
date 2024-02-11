#include <gtkpoly/window.hxx>

void Poly::Window::set_child(std::shared_ptr<Widget> widget) {
	set_child(*widget);
	root_widget = std::move(widget);
}

std::shared_ptr<Poly::Window>
Poly::WindowManager::new_window_with_tag(const std::string &tag) {
	auto window = std::make_shared<Window>();
	windows.insert({tag, window});
	return window;
}

std::shared_ptr<Poly::Window>
Poly::WindowManager::find_window_with_tag(const std::string &tag) const {
	return windows.find(tag)->second;
}
