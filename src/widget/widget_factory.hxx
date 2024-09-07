//
// Created by kenym on 08/02/24.
//

#ifndef GTKPOLY_WIDGET_FACTORY_HXX
#define GTKPOLY_WIDGET_FACTORY_HXX

#include "../rpc/widget/widget.np.hxx"

#include <glibmm/refptr.h>
#include <gtkpoly/application.hxx>

namespace Poly {

Glib::RefPtr<Gtk::Widget> make_widget(Rpc::Widget &widget,
									  std::shared_ptr<Application> app);

}

#endif // GTKPOLY_WIDGET_FACTORY_HXX
