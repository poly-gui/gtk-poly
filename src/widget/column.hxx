#ifndef GTKPOLY_COLUMN_HXX
#define GTKPOLY_COLUMN_HXX

#include <glibmm/refptr.h>
#include <gtkmm/box.h>
#include <memory>

#include "../application.hxx"
#include "../rpc/widget/column.np.hxx"

namespace Poly {

class Column final : public Gtk::Box {
	Gtk::Align horizontal_alignment;
	Gtk::Align vertical_alignment;

  protected:
	Column(const Rpc::Column &msg, std::shared_ptr<_Application> app);

  public:
	int32_t tag;

	static Glib::RefPtr<Column> create(const Rpc::Column &msg,
									   std::shared_ptr<_Application> app);

	using Box::append;
	void append(const Glib::RefPtr<Widget> &widget);
};

} // namespace Poly

#endif // GTKPOLY_COLUMN_HXX
