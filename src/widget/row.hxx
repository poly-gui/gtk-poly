#ifndef ROW_HXX
#define ROW_HXX

#include <gtkmm/box.h>
#include <gtkpoly/application.hxx>

#include "../messages/widgets/row.np.hxx"

namespace Poly {

class Row final : public Gtk::Box {
	Gtk::Align horizontal_alignment;
	Gtk::Align vertical_alignment;
	std::vector<std::shared_ptr<Widget>> children;

	bool has_spacer;

  public:
	int32_t tag;

	explicit Row(const Message::Row &row);

	using Box::append;
	void append(std::shared_ptr<Widget> widget);
};

std::unique_ptr<Row> make_row(const Message::Row &row,
							  std::shared_ptr<Application> app);

} // namespace Poly

#endif // ROW_HXX
