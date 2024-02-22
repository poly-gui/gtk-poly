#ifndef SPACER_HXX
#define SPACER_HXX

#include <gtkmm/label.h>

namespace Poly {

class HorizontalSpacer final : public Gtk::Label {
  public:
	HorizontalSpacer();
};

class VerticalSpacer final : public Gtk::Label {
public:
	VerticalSpacer();
};

} // namespace Poly

#endif // SPACER_HXX
