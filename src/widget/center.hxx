//
// Created by kenym on 10/02/24.
//

#ifndef GTKPOLY_CENTER_HXX
#define GTKPOLY_CENTER_HXX

#include <glibmm/refptr.h>
#include <gtkmm/widget.h>
#include <memory>

#include "../application.hxx"
#include "../rpc/widget/center.np.hxx"

namespace Poly {

Glib::RefPtr<Gtk::Widget> make_center(const Rpc::Center &center,
									  std::shared_ptr<_Application> app);

}

#endif // GTKPOLY_CENTER_HXX
