#ifndef GTKPOLY__WIDGET_HXX
#define GTKPOLY__WIDGET_HXX

#include "glibmm/refptr.h"
#include "gtkmm/widget.h"
namespace Poly {

class MultiChildrenWidget {
  public:
	MultiChildrenWidget() = default;

	virtual void append_widget(const Glib::RefPtr<Gtk::Widget> &widget) = 0;

	virtual void
	insert_widget_before(const Glib::RefPtr<Gtk::Widget> &widget,
						 const Glib::RefPtr<Gtk::Widget> &before_widget) = 0;
};

} // namespace Poly

#endif
