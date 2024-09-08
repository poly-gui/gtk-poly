#ifndef APPLICATION_HXX
#define APPLICATION_HXX

#include <filesystem>
#include <gtkmm/application.h>

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
class Application : public Gtk::Application {
  public:
	/**
	 * \brief Creates an instance of a Poly application.
	 * \param config Configuration for this application.
	 * \return A shared pointer to the application instance.
	 * \see Poly::ApplicationConfig
	 */
	static std::shared_ptr<Application> create(const ApplicationConfig &config);

	/**
	 * \brief Starts running the application and returns when the application
	 * quits. \return The exit code of the application. 0 indicates success.
	 */
	virtual int start() = 0;

  protected:
	using Gtk::Application::Application;
};

} // namespace Poly

#endif // APPLICATION_HXX
