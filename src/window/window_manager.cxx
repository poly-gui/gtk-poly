#include <gtkpoly/window.hxx>

std::shared_ptr<Poly::Window>
Poly::WindowManager::new_window_with_tag(const std::string &tag) {
	auto window = std::make_shared<Window>();
	windows.insert({tag, window});
	return window;
}

std::shared_ptr<Poly::Window> Poly::WindowManager::find_window_with_tag(const std::string &tag) {
	return windows.find(tag)->second;
}