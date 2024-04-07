//
// Created by kenym on 10/02/24.
//

#ifndef GTKPOLY_WIDGET_UPDATER_HXX
#define GTKPOLY_WIDGET_UPDATER_HXX

#include <gtkmm/widget.h>
#include <optional>

#include "../messages/widgets/widget.np.hxx"
#include "nanopack/any.hxx"

namespace Poly {

void update_widget(Gtk::Widget &old_widget, Message::Widget &new_config,
				   const std::optional<NanoPack::Any> &args);

}

#endif // GTKPOLY_WIDGET_UPDATER_HXX
