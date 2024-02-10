//
// Created by kenym on 08/02/24.
//

#ifndef GTKPOLY_WIDGET_FACTORY_HXX
#define GTKPOLY_WIDGET_FACTORY_HXX
#include "../messages/widgets/widget.np.hxx"

#include <gtkpoly/application.hxx>
#include <memory>

namespace Poly {

std::unique_ptr<Gtk::Widget> make_widget(Message::Widget &widget,
										 const Application &app);

}

#endif // GTKPOLY_WIDGET_FACTORY_HXX
