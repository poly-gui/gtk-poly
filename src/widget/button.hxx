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

#include "../messages/widgets/button/button.np.hxx"

namespace Poly {

class Button final : public Gtk::Button {
  protected:
	Button(const Message::Button &button, std::shared_ptr<Application> app);

  public:
	int32_t tag;

	int32_t on_click_handle;

	static Glib::RefPtr<Button> create(const Message::Button &button,
									   std::shared_ptr<Application> app);

	void update(const Message::Button &msg);
};

} // namespace Poly

#endif // GTKPOLY_BUTTON_HXX