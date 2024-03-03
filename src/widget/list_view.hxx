#ifndef LIST_VIEW_HXX
#define LIST_VIEW_HXX

#include <gtkmm/listview.h>

namespace Poly {

class ListView final : public Gtk::ListView {
  public:
	int32_t tag;
};

} // namespace Poly

#endif // LIST_VIEW_HXX
