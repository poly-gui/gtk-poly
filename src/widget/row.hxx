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

  protected:
	Row(const Message::Row &row, std::shared_ptr<Application> app);

  public:
	int32_t tag;

	static Glib::RefPtr<Row> create(const Message::Row &msg,
									std::shared_ptr<Application> app);

	using Box::append;
	void append(std::shared_ptr<Widget> widget);
};

} // namespace Poly

#endif // ROW_HXX
