#include "widget_updater.hxx"
#include <iostream>

#include "../rpc/widget/button.np.hxx"
#include "../rpc/widget/list_view_batch_operations.np.hxx"
#include "../rpc/widget/text.np.hxx"
#include "../rpc/widget/text_field.np.hxx"
#include "button.hxx"
#include "list_view.hxx"
#include "nanopack/message.hxx"
#include "text.hxx"
#include "text_field.hxx"

void Poly::update_widget(Gtk::Widget &old_widget, Rpc::Widget &new_config,
						 NanoPack::Message *args) {
	switch (new_config.type_id()) {
	case Rpc::Text::TYPE_ID:
		static_cast<Text &>(old_widget)
			.update(static_cast<Rpc::Text &>(new_config));
		break;

	case Rpc::Button::TYPE_ID:
		static_cast<Button &>(old_widget)
			.update(static_cast<Rpc::Button &>(new_config));
		break;

	case Rpc::TextField::TYPE_ID:
		static_cast<TextField &>(old_widget)
			.update(static_cast<Rpc::TextField &>(new_config));
		break;

	case Rpc::ListView::TYPE_ID:
		if (args) {
			static_cast<ListView &>(old_widget)
				.update(static_cast<Rpc::ListView &>(new_config),
						*static_cast<Rpc::ListViewBatchOperations *>(args));
		}
		break;

	default:
#ifdef DEBUG
		std::cout << "WARNING: unsupported update operation for widget #"
				  << new_config.type_id();
#endif
		break;
	}
}
