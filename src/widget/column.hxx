//
// Created by kenym on 10/02/24.
//

#ifndef GTKPOLY_COLUMN_HXX
#define GTKPOLY_COLUMN_HXX

#include <glibmm/refptr.h>
#include <gtkmm/box.h>
#include <gtkpoly/application.hxx>
#include <memory>

#include "../messages/widgets/column.np.hxx"

namespace Poly {

class Column final : public Gtk::Box {
	Gtk::Align horizontal_alignment;
	Gtk::Align vertical_alignment;
	std::vector<std::shared_ptr<Widget>> children;

	bool has_spacer;

  protected:
	Column(const Message::Column &msg, std::shared_ptr<Application> app);

  public:
	int32_t tag;

	static Glib::RefPtr<Column> create(const Message::Column &msg,
									   std::shared_ptr<Application> app);

	using Box::append;
	void append(std::shared_ptr<Widget> widget);
};

} // namespace Poly

#endif // GTKPOLY_COLUMN_HXX
