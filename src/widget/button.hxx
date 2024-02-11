//
// Created by kenym on 10/02/24.
//

#ifndef GTKPOLY_BUTTON_HXX
#define GTKPOLY_BUTTON_HXX

#include <gtkmm/button.h>
#include <gtkpoly/application.hxx>
#include <memory>

#include "../messages/widgets/button/button.np.hxx"

namespace Poly {

class Button final : public Gtk::Button {
public:
	int32_t tag;

	using Gtk::Button::Button;
};

std::unique_ptr<Button> make_button(const Message::Button &button,
                                    std::shared_ptr<Application> app);

void update_button(Button &button, const Message::Button &new_config);

} // namespace Poly

#endif // GTKPOLY_BUTTON_HXX