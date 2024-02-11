#ifndef APPLICATION_HXX
#define APPLICATION_HXX

#include <filesystem>
#include <gtkmm/application.h>

#include "../../src/messages/create_window.np.hxx"
#include "../../src/messages/widgets/create_widget.np.hxx"
#include "../../src/messages/widgets/update_widget.np.hxx"
#include "../../src/portable_layer.hxx"
#include "../../src/widget/widget_registry.hxx"
#include "window.hxx"

namespace Poly {

struct ApplicationConfig {
	std::string application_id;
	std::filesystem::path app_dir_path;
	Gio::Application::Flags flags;
};

class Application : public Gtk::Application,
					public std::enable_shared_from_this<Application> {
	struct Private {};

	ApplicationConfig config;
	WindowManager window_manager;
	WidgetRegistry _widget_registry;
	PortableLayer _portable_layer;

	void handle_message(std::unique_ptr<NanoPack::Message> msg);

	void read_incoming_messages();

	void create_window(std::unique_ptr<NanoPack::Message> msg);

	void create_widget(std::unique_ptr<NanoPack::Message> msg);

	void update_widget(std::unique_ptr<NanoPack::Message> msg);

	void cleanup();

	void on_activate() override;

  public:
	Application(Private, const ApplicationConfig &config);

	static std::shared_ptr<Application> create(const ApplicationConfig &config);

	PortableLayer &portable_layer();

	WidgetRegistry &widget_registry();

	int start();
};

} // namespace Poly

#endif // APPLICATION_HXX
