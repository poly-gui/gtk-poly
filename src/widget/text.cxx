//
// Created by kenym on 08/02/24.
//

#include "text.hxx"

#include <glibmm/objectbase.h>

Poly::Text::Text(const Message::Text &msg)
	: Glib::ObjectBase(typeid(Poly::Text)), Label(),
	  tag(msg.tag.has_value() ? *msg.tag : -1) {
	set_halign(Gtk::Align::START);
	set_valign(Gtk::Align::START);
	set_text(msg.content);
}

Glib::RefPtr<Poly::Text> Poly::Text::create(const Message::Text &msg) {
	return Glib::make_refptr_for_instance<Text>(new Text(msg));
}

void Poly::update_text(Text &text, const Message::Text &new_config) {
	text.set_text(new_config.content);
}
