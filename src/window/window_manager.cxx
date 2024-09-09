#include <gtkpoly/window.hxx>
#include <iostream>

size_t Poly::WindowManager::active_window_count() const {
	return windows.size();
}

std::shared_ptr<Gtk::Window>
Poly::WindowManager::new_window_with_tag(const std::string &tag) {
	auto window = std::make_shared<Gtk::Window>();
	std::cout << "tag " << tag << std::endl;
	windows.insert({tag, window});
	return window;
}

std::shared_ptr<Gtk::Window>
Poly::WindowManager::find_window_with_tag(const std::string &tag) const {
	return windows.find(tag)->second;
}

void Poly::WindowManager::remove_window_by_tag(const std::string &tag) {
	if (const auto entry = windows.find(tag); entry != windows.end()) {
		windows.erase(entry);
	}
}
