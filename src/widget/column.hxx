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
  private:
	std::vector<std::unique_ptr<Widget>> children;

  public:
	explicit Column(const Message::Column &column);

	int32_t tag;

	using Box::append;
	void append(std::unique_ptr<Widget> widget);
};

std::unique_ptr<Column> make_column(const Message::Column &column,
									const Application &app);

} // namespace Poly

#endif // GTKPOLY_COLUMN_HXX
