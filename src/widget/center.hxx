//
// Created by kenym on 10/02/24.
//

#ifndef GTKPOLY_CENTER_HXX
#define GTKPOLY_CENTER_HXX

#include <gtkmm/widget.h>
#include <gtkpoly/application.hxx>
#include <memory>

#include "../messages/widgets/center.np.hxx"

namespace Poly {

std::shared_ptr<Gtk::Widget> make_center(const Message::Center &center,
										 std::shared_ptr<Application> app);

}

#endif // GTKPOLY_CENTER_HXX
