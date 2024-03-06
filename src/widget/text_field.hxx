#ifndef GTKPOLY_TEXT_FIELD_HXX
#define GTKPOLY_TEXT_FIELD_HXX

#include <glibmm/refptr.h>
#include <gtkmm/text.h>
#include <gtkpoly/application.hxx>

#include "../messages/widgets/text_field/text_field.np.hxx"

namespace Poly {

class TextField final : public Gtk::Text {
  protected:
	TextField(const Message::TextField &msg, std::shared_ptr<Application> app);

  public:
	int32_t tag;
	int32_t on_value_changed_handle;

	static Glib::RefPtr<TextField> create(const Message::TextField &msg,
										  std::shared_ptr<Application> app);

	void update(const Message::TextField &msg);
};

void update_text_field(TextField &msg, const Message::TextField &new_config);

} // namespace Poly

#endif // GTKPOLY_TEXT_FIELD_HXX
