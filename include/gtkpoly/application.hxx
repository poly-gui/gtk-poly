#ifndef APPLICATION_HXX
#define APPLICATION_HXX

#include <filesystem>
#include <gtkmm/application.h>
#include <nanopack/message.hxx>

#include "../../src/portable_layer.hxx"
#include "../../src/widget/widget_registry.hxx"
#include "window.hxx"

namespace Poly {

/**
 * \brief Configuration options available for a Poly GTK Application.
 */
struct ApplicationConfig {
	/**
	 * \brief GTK Application ID for this application.
	 * https://developer.gnome.org/documentation/tutorials/application-id.html
	 */
	std::string application_id;

	/**
	 * \brief Absolute path to the data directory for this application. Usually
	 * it's /var/lib/<myapp>/ or /var/lib64/<myapp>/
	 */
	std::filesystem::path app_dir_path;

	/**
	 * \brief Flags that define the behavior of the GTK application.
	 * \link https://docs.gtk.org/gio/flags.ApplicationFlags.html \endlink
	 */
	Gio::Application::Flags flags;
};

/**
 * \brief A wrapper of \ref Gtk::Application
 * that handles and manages Poly-specific components and resources:
 *
 * - Managing the portable layer process
 * - Handling messages from the portable layer, including creating windows and
 * widgets.
 * - Storing references to tagged widgets
 * - Managing active Poly windows
 *
 * To creata a new instance, use \ref Poly::Application::create, passing along
 * the configuration for the application.
 *
 * \see Gtk::Application
 * \see Poly::Application::create
 */
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

	void on_poly_window_destroyed(const std::string &window_tag);

  public:
	Application(Private, const ApplicationConfig &config);

	/**
	 * \brief Creates an instance of a Poly application.
	 * \param config Configuration for this application.
	 * \return A shared pointer to the application instance.
	 * \see Poly::ApplicationConfig
	 */
	static std::shared_ptr<Application> create(const ApplicationConfig &config);

	PortableLayer &portable_layer();

	WidgetRegistry &widget_registry();

	/**
	 * \brief Starts running the application and returns when the application
	 * quits. \return The exit code of the application. 0 indicates success.
	 */
	int start();
};

} // namespace Poly

#endif // APPLICATION_HXX
