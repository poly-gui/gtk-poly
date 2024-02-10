#ifndef APPLICATION_HXX
#define APPLICATION_HXX

#include <filesystem>
#include <gtkmm/application.h>

#include "../../src/messages/create_window.np.hxx"
#include "../../src/messages/widgets/create_widget.np.hxx"
#include "../../src/portable_layer.hxx"
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

	void create_window(const Message::CreateWindow *msg);

	void create_widget(Message::CreateWidget *msg) const;

	void cleanup();

	void on_activate() override;

  public:
	explicit Application(const ApplicationConfig &config);

	int start();
};

} // namespace Poly

#endif // APPLICATION_HXX
