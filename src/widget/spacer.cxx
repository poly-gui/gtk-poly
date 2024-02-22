#include "spacer.hxx"

#if DEBUG
#define SPACER_LABEL "[SPACER]"
#else
#define SPACER_LABEL ""
#endif

Poly::HorizontalSpacer::HorizontalSpacer() : Label(SPACER_LABEL) {
	set_hexpand(true);
	set_halign(Gtk::Align::FILL);
}

Poly::VerticalSpacer::VerticalSpacer() : Label(SPACER_LABEL) {
	set_vexpand(true);
	set_valign(Gtk::Align::FILL);
}
