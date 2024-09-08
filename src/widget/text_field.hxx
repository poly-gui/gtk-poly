#ifndef GTKPOLY_TEXT_FIELD_HXX
#define GTKPOLY_TEXT_FIELD_HXX

#include <glibmm/refptr.h>
#include <gtkmm/text.h>
#include <gtkpoly/application.hxx>

#include "../application.hxx"
#include "../rpc/widget/text_field.np.hxx"

namespace Poly {

class TextField final : public Gtk::Text {
  protected:
	TextField(const Rpc::TextField &msg, std::shared_ptr<_Application> app);

  public:
	int32_t tag;
	int32_t on_value_changed_handle;

	static Glib::RefPtr<TextField> create(const Rpc::TextField &msg,
										  std::shared_ptr<_Application> app);

	void update(const Rpc::TextField &msg);
};

void update_text_field(TextField &msg, const Rpc::TextField &new_config);

} // namespace Poly

#endif // GTKPOLY_TEXT_FIELD_HXX
