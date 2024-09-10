#include <giomm/liststore.h>
#include <glibmm.h>
#include <gtkmm/label.h>
#include <gtkmm/noselection.h>
#include <gtkmm/signallistitemfactory.h>
#include <memory>
#include <optional>

#include "../dimens.hxx"
#include "../rpc/widget/list_view_delete_operation.np.hxx"
#include "../rpc/widget/list_view_insert_operation.np.hxx"
#include "../rpc/widget/list_view_item.np.hxx"
#include "../rpc/widget/list_view_item_config.np.hxx"
#include "glibmm/refptr.h"
#include "list_view.hxx"
#include "nanopack/message.hxx"
#include "widget_factory.hxx"

Poly::__ListViewPlaceholderItemObjectDoNotUseOrGetFired__::
	__ListViewPlaceholderItemObjectDoNotUseOrGetFired__()
	: Glib::ObjectBase(
		  typeid(__ListViewPlaceholderItemObjectDoNotUseOrGetFired__)) {}

Glib::RefPtr<Poly::__ListViewPlaceholderItemObjectDoNotUseOrGetFired__>
Poly::__ListViewPlaceholderItemObjectDoNotUseOrGetFired__::create() {
	return Glib::make_refptr_for_instance<
		__ListViewPlaceholderItemObjectDoNotUseOrGetFired__>(
		new __ListViewPlaceholderItemObjectDoNotUseOrGetFired__());
}

Poly::ListView::ListView(const Rpc::ListView &list_view,
						 std::shared_ptr<_Application> app)
	: app(std::move(app)), on_create(list_view.on_create),
	  on_bind(list_view.on_bind),
	  tag(list_view.tag.has_value() ? *list_view.tag : -1),
	  item_height(list_view.item_height) {
	const uint32_t item_count = list_view.sections[0];
	const int desired_width = static_cast<int>(round(list_view.width));
	const int desired_height = static_cast<int>(round(list_view.height));

	auto factory = Gtk::SignalListItemFactory::create();
	factory->signal_setup().connect(
		[&](const Glib::RefPtr<Gtk::ListItem> &list_item) {
			create_list_item(list_item);
		});
	factory->signal_bind().connect(
		[&](const Glib::RefPtr<Gtk::ListItem> &list_item) {
			bind_list_item(list_item);
		});

	store = Gio::ListStore<
		__ListViewPlaceholderItemObjectDoNotUseOrGetFired__>::create();
	for (uint32_t i = 0; i < item_count; i++) {
		store->append(
			__ListViewPlaceholderItemObjectDoNotUseOrGetFired__::create());
	}

	gtk_list_view = std::make_shared<Gtk::ListView>(
		Gtk::NoSelection::create(store), factory);
	gtk_list_view->set_size_request(300, 300);

	set_child(*gtk_list_view);

	if (desired_width == Dimension::FILL_PARENT) {
		set_halign(Gtk::Align::FILL);
	}
	if (desired_height == Dimension::FILL_PARENT) {
		set_valign(Gtk::Align::FILL);
		set_vexpand(true);
	}
}

Glib::RefPtr<Poly::ListView>
Poly::ListView::create(const Rpc::ListView &list_view,
					   std::shared_ptr<_Application> app) {
	return Glib::make_refptr_for_instance<ListView>(
		new ListView(list_view, std::move(app)));
}

void Poly::ListView::create_list_item(
	const Glib::RefPtr<Gtk::ListItem> &list_item) {
	const Rpc::ListViewItemConfig config(std::nullopt, std::nullopt,
										 std::nullopt);
	std::unique_ptr<NanoPack::Message> result =
		app->portable_layer()
			.invoke_callback(on_create,
							 std::make_unique<Rpc::ListViewItemConfig>(
								 std::nullopt, std::nullopt, std::nullopt))
			.get();

	const auto item = static_cast<Rpc::ListViewItem *>(result.get());
	item_tags.insert({list_item, item->item_tag});

	const Glib::RefPtr<Widget> item_widget =
		make_widget(item->get_widget(), app);
	item_widget->set_size_request(-1, item_height);

	list_item->set_child(*item_widget);
}

void Poly::ListView::update(const Rpc::ListView &msg,
							const Rpc::ListViewBatchOperations &operations) {
	for (const std::unique_ptr<Rpc::ListViewOperation> &operation :
		 operations.operations) {
		switch (operation->type_id()) {
		case Rpc::ListViewInsertOperation::TYPE_ID: {
			auto ins_op =
				static_cast<Rpc::ListViewInsertOperation *>(operation.get());
			for (auto i : ins_op->insert_at) {
				store->insert(
					i, __ListViewPlaceholderItemObjectDoNotUseOrGetFired__::
						   create());
			}
			break;
		}

		case Rpc::ListViewDeleteOperation::TYPE_ID: {
			auto del_op =
				static_cast<Rpc::ListViewDeleteOperation *>(operation.get());
			for (auto i : del_op->delete_at) {
				store->remove(i);
			}
			break;
		}

		default:
			break;
		}
	}
}

void Poly::ListView::bind_list_item(
	const Glib::RefPtr<Gtk::ListItem> &list_item) {
	const auto entry = item_tags.find(list_item);
	if (entry == item_tags.end())
		return;

	const uint32_t item_tag = entry->second;

	app->portable_layer()
		.invoke_void_callback(on_bind,
							  std::make_unique<Rpc::ListViewItemConfig>(
								  0, list_item->get_position(), item_tag))
		.get();
}
