#ifndef GTKPOLY_TEXT_FIELD_HXX
#define GTKPOLY_TEXT_FIELD_HXX

#include <gtkmm/text.h>
#include <gtkpoly/application.hxx>

#include "../messages/widgets/text_field/text_field.np.hxx"

namespace Poly {

class TextField final : public Gtk::Text {
  public:
	int32_t tag;
	int32_t on_value_changed_handle;

	TextField(const Message::TextField &text_field,
			  std::shared_ptr<Application> app);
};

std::unique_ptr<TextField> make_text_field(const Message::TextField &text_field,
										   std::shared_ptr<Application> app);

void update_text_field(TextField &text_field,
					   const Message::TextField &new_config);

} // namespace Poly

#endif // GTKPOLY_TEXT_FIELD_HXX
