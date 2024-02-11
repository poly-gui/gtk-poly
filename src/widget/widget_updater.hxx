//
// Created by kenym on 10/02/24.
//

#ifndef GTKPOLY_WIDGET_UPDATER_HXX
#define GTKPOLY_WIDGET_UPDATER_HXX

#include <gtkmm/widget.h>

#include "../messages/widgets/widget.np.hxx"

namespace Poly {

void update_widget(Gtk::Widget &old_widget, Message::Widget &new_config);

}

#endif // GTKPOLY_WIDGET_UPDATER_HXX
