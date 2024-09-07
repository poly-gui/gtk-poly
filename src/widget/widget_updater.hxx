//
// Created by kenym on 10/02/24.
//

#ifndef GTKPOLY_WIDGET_UPDATER_HXX
#define GTKPOLY_WIDGET_UPDATER_HXX

#include <gtkmm/widget.h>

#include "../rpc/widget/widget.np.hxx"
#include "nanopack/message.hxx"

namespace Poly {

void update_widget(Gtk::Widget &old_widget, Rpc::Widget &new_config,
				   NanoPack::Message *args);

}

#endif // GTKPOLY_WIDGET_UPDATER_HXX
