//
// Created by kenym on 10/02/24.
//

#ifndef GTKPOLY_COLUMN_HXX
#define GTKPOLY_COLUMN_HXX

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

  public:
	int32_t tag;

	explicit Column(const Message::Column &column);

	using Box::append;
	void append(std::shared_ptr<Widget> widget);
};

std::unique_ptr<Column> make_column(const Message::Column &column,
									std::shared_ptr<Application> app);

} // namespace Poly

#endif // GTKPOLY_COLUMN_HXX
