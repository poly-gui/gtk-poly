//
// Created by kenym on 10/02/24.
//

#ifndef GTKPOLY_BUTTON_HXX
#define GTKPOLY_BUTTON_HXX

#include <glibmm/refptr.h>
#include <gtkmm/button.h>
#include <gtkpoly/application.hxx>
#include <memory>
#include <sigc++/connection.h>

#include "../application.hxx"
#include "../rpc/widget/button.np.hxx"

namespace Poly {

class Button final : public Gtk::Button {
  protected:
	Button(const Rpc::Button &button, std::shared_ptr<_Application> app);

  public:
	int32_t tag;

	int32_t on_click_handle;

	static Glib::RefPtr<Button> create(const Rpc::Button &button,
									   std::shared_ptr<_Application> app);

	void update(const Rpc::Button &msg);
};

} // namespace Poly

#endif // GTKPOLY_BUTTON_HXX
