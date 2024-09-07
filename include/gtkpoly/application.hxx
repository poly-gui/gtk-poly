#ifndef APPLICATION_HXX
#define APPLICATION_HXX

#include <filesystem>
#include <gtkmm/application.h>
#include <nanopack/message.hxx>

#include "../../src/rpc/native_layer_service.np.hxx"
#include "../../src/rpc/portable_layer_service.np.hxx"
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
					public std::enable_shared_from_this<Application>,
					private Rpc::NativeLayerServiceServer {
	struct Private {};

	ApplicationConfig config;
	WindowManager window_manager;
	WidgetRegistry _widget_registry;

	Rpc::PortableLayerServiceClient _portable_layer;
	int portable_layer_pid;
	int portable_layer_stdin_handle;
	int portable_layer_stdout_handle;

	void spawn_portable_layer();

	void create_window(const std::string &title, const std::string &description,
					   int32_t width, int32_t height,
					   const std::string &tag) override;

	void clear_window(const std::string &window_tag) override;

	void create_widget(std::unique_ptr<Rpc::Widget> widget,
					   const std::string &window_tag) override;

	void append_new_widget(std::unique_ptr<Rpc::Widget> child,
						   uint32_t parent_tag) override;

	void insert_widget_before(std::unique_ptr<Rpc::Widget> widget,
							  std::unique_ptr<Rpc::Widget> before_widget,
							  uint32_t parent_tag) override;

	void update_widget(uint32_t tag, std::unique_ptr<Rpc::Widget> widget,
					   std::unique_ptr<NanoPack::Message> args) override;

	void
	update_widgets(const std::vector<uint32_t> &tag,
				   const std::vector<std::unique_ptr<Rpc::Widget>> &widgets,
				   std::unique_ptr<NanoPack::Message> args) override;

	void remove_widget(uint32_t tag) override;

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

	Rpc::PortableLayerServiceClient &portable_layer();

	WidgetRegistry &widget_registry();

	/**
	 * \brief Starts running the application and returns when the application
	 * quits. \return The exit code of the application. 0 indicates success.
	 */
	int start();
};

} // namespace Poly

#endif // APPLICATION_HXX
