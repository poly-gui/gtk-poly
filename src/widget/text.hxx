//
// Created by kenym on 08/02/24.
//

#ifndef GTKPOLY_TEXT_HXX
#define GTKPOLY_TEXT_HXX

#include <gtkmm/label.h>
#include <gtkpoly/application.hxx>
#include <memory>

#include "../messages/widgets/text.np.hxx"

namespace Poly {
class Text final : public Gtk::Label {
  public:
	int32_t tag;
};

std::unique_ptr<Text> make_text(const Message::Text &msg,
								std::shared_ptr<Application> app);

void update_text(Text &text, const Message::Text &new_config);

} // namespace Poly

#endif // GTKPOLY_TEXT_HXX
