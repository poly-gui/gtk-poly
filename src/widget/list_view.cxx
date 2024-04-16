#include <giomm/liststore.h>
#include <glibmm.h>
#include <gtkmm/label.h>
#include <gtkmm/noselection.h>
#include <gtkmm/signallistitemfactory.h>

#include "../dimens.hxx"
#include "../messages/widgets/list_view/list_view_delete_operation.np.hxx"
#include "../messages/widgets/list_view/list_view_insert_operation.np.hxx"
#include "../messages/widgets/list_view/list_view_item.np.hxx"
#include "../messages/widgets/list_view/list_view_item_config.np.hxx"
#include "../messages/widgets/update_widgets.np.hxx"
#include "glibmm/refptr.h"
#include "list_view.hxx"
#include "widget_factory.hxx"
#include "widget_updater.hxx"

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

Poly::ListView::ListView(const Message::ListView &list_view,
						 std::shared_ptr<Application> app)
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
Poly::ListView::create(const Message::ListView &list_view,
					   std::shared_ptr<Application> app) {
	return Glib::make_refptr_for_instance<ListView>(
		new ListView(list_view, std::move(app)));
}

void Poly::ListView::create_list_item(
	const Glib::RefPtr<Gtk::ListItem> &list_item) {
	const Message::ListViewItemConfig config(std::nullopt, std::nullopt,
											 std::nullopt);
	const NanoPack::Any result =
		app->portable_layer()
			.invoke_callback_with_result(on_create, config)
			.get();

	int bytes_read;
	const Message::ListViewItem item(result.as_reader(), bytes_read);

	item_tags.insert({list_item, item.item_tag});

	const Glib::RefPtr<Widget> item_widget =
		make_widget(item.get_widget(), app);
	item_widget->set_size_request(-1, item_height);

	list_item->set_child(*item_widget);
}

void Poly::ListView::update(
	const Message::ListView &msg,
	const Message::ListViewBatchOperations &operations) {
	for (const std::unique_ptr<Message::ListViewOperation> &operation :
		 operations.operations) {
		switch (operation->type_id()) {
		case Message::ListViewInsertOperation::TYPE_ID: {
			auto ins_op = static_cast<Message::ListViewInsertOperation *>(
				operation.get());
			for (auto i : ins_op->insert_at) {
				store->insert(
					i, __ListViewPlaceholderItemObjectDoNotUseOrGetFired__::
						   create());
			}
			break;
		}

		case Message::ListViewDeleteOperation::TYPE_ID: {
			auto del_op = static_cast<Message::ListViewDeleteOperation *>(
				operation.get());
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

	const Message::ListViewItemConfig config(0, list_item->get_position(),
											 item_tag);

	const NanoPack::Any result =
		app->portable_layer()
			.invoke_callback_with_result(on_bind, config)
			.get();

	int bytes_read;
	const Message::UpdateWidgets updates(result.as_reader(), bytes_read);
	for (const Message::UpdateWidget &update : updates.updates) {
		Glib::RefPtr<Gtk::Widget> widget =
			app->widget_registry().find_widget(*update.get_widget().tag);
		update_widget(*widget, update.get_widget(), update.args);
	}
}
