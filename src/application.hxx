#ifndef GTKPOLY__SRC_APPLICATION_HXX
#define GTKPOLY__SRC_APPLICATION_HXX

#include "gtkmm/label.h"
#include "nanopack/rpc.hxx"
#include "rpc/native_layer_service.np.hxx"
#include "rpc/portable_layer_service.np.hxx"
#include "widget/widget_registry.hxx"
#include <gtkpoly/application.hxx>
#include <gtkpoly/window.hxx>
#include <optional>

namespace Poly {

class _Application : public Poly::Application,
					 public std::enable_shared_from_this<_Application>,
					 private Rpc::NativeLayerServiceServer {
	struct Private {};

	ApplicationConfig config;
	WindowManager window_manager;
	WidgetRegistry _widget_registry;

	std::optional<NanoPack::StandardIoChannel> rpc_channel;

	std::mutex ui_mutex;

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
	_Application(const ApplicationConfig &config);

	Rpc::PortableLayerServiceClient &portable_layer();

	WidgetRegistry &widget_registry();

	/**
	 * \brief Starts running the application and returns when the application
	 * quits. \return The exit code of the application. 0 indicates success.
	 */
	int start() override;
};

} // namespace Poly

#endif
