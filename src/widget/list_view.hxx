#ifndef LIST_VIEW_HXX
#define LIST_VIEW_HXX

#include <glibmm/refptr.h>
#include <gtkmm/listitem.h>
#include <gtkmm/listview.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/signallistitemfactory.h>
#include <gtkpoly/application.hxx>

#include "../messages/widgets/list_view/list_view.np.hxx"
#include "../messages/widgets/list_view/list_view_batch_operations.np.hxx"
#include "giomm/liststore.h"

namespace Poly {

class __ListViewPlaceholderItemObjectDoNotUseOrGetFired__
	: public Glib::Object {
  protected:
	__ListViewPlaceholderItemObjectDoNotUseOrGetFired__();

  public:
	static Glib::RefPtr<__ListViewPlaceholderItemObjectDoNotUseOrGetFired__>
	create();
};

class ListView final : public Gtk::ScrolledWindow {
	std::shared_ptr<Gtk::ListView> gtk_list_view;
	std::shared_ptr<Application> app;
	Glib::RefPtr<
		Gio::ListStore<__ListViewPlaceholderItemObjectDoNotUseOrGetFired__>>
		store;

	int32_t on_create;
	int32_t on_bind;

	std::unordered_map<Glib::RefPtr<Gtk::ListItem>, uint32_t> item_tags;

	void create_list_item(const Glib::RefPtr<Gtk::ListItem> &list_item);

	void bind_list_item(const Glib::RefPtr<Gtk::ListItem> &list_item);

  protected:
	ListView(const Message::ListView &list_view,
			 std::shared_ptr<Application> app);

  public:
	int32_t tag;

	uint32_t item_height;

	static Glib::RefPtr<ListView> create(const Message::ListView &list_view,
										 std::shared_ptr<Application> app);

	void update(const Message::ListView &msg,
				const Message::ListViewBatchOperations &operations);
};

} // namespace Poly

#endif // LIST_VIEW_HXX
