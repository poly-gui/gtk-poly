#include "text.hxx"

#include <glibmm/objectbase.h>
#include <iostream>

Poly::Text::Text(const Rpc::Text &msg)
	: Glib::ObjectBase(typeid(Poly::Text)), Label(msg.content),
	  tag(msg.tag.has_value() ? *msg.tag : -1) {
	set_halign(Gtk::Align::START);
	set_valign(Gtk::Align::START);
	std::cout << "text content" << msg.content << std::endl;
}

Glib::RefPtr<Poly::Text> Poly::Text::create(const Rpc::Text &msg) {
	return Glib::make_refptr_for_instance<Text>(new Text(msg));
}

void Poly::Text::update(const Rpc::Text &msg) { set_text(msg.content); }

void Poly::update_text(Text &text, const Rpc::Text &new_config) {
	text.set_text(new_config.content);
}
