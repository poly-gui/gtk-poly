#ifndef GTKPOLY_ROW_HXX
#define GTKPOLY_ROW_HXX

#include <gtkmm/box.h>

#include "../application.hxx"
#include "../rpc/widget/row.np.hxx"

namespace Poly {

class Row final : public Gtk::Box {
	Gtk::Align horizontal_alignment;
	Gtk::Align vertical_alignment;

  protected:
	Row(const Rpc::Row &row, std::shared_ptr<_Application> app);

  public:
	int32_t tag;

	static Glib::RefPtr<Row> create(const Rpc::Row &msg,
									std::shared_ptr<_Application> app);

	using Box::append;
	void append(const Glib::RefPtr<Widget> &widget);
};

} // namespace Poly

#endif // GTKPOLY_ROW_HXX
