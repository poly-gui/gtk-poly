#ifndef APPLICATION_HXX
#define APPLICATION_HXX

#include <gtkmm/application.h>
#include <filesystem>

#include "../../src/portable_layer.hxx"
#include "../../src/messages/create_window.np.hxx"
#include "window.hxx"

namespace Poly {

struct ApplicationConfig {
	std::string application_id;
	std::filesystem::path app_dir_path;
	Gio::Application::Flags flags;
};

class Application : public Gtk::Application {
  private:
	ApplicationConfig config;
	WindowManager window_manager;
	PortableLayer portable_layer;

	void handle_message(std::unique_ptr<NanoPack::Message> msg);

	void read_incoming_messages();

	void create_window(const CreateWindow *msg);

	void cleanup();

	void on_activate() override;

  public:
	explicit Application(const ApplicationConfig &config);

	int start();
};

} // namespace Poly

#endif // APPLICATION_HXX
