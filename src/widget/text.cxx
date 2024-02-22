//
// Created by kenym on 08/02/24.
//

#include "text.hxx"

#include <glibmm/main.h>

std::unique_ptr<Poly::Text> Poly::make_text(const Message::Text &msg,
                                            std::shared_ptr<Application> app) {
	auto text = std::make_unique<Text>();
	if (msg.tag.has_value()) {
		text->tag = *msg.tag;
	}
	text->set_halign(Gtk::Align::START);
	text->set_valign(Gtk::Align::START);
	text->set_text(msg.content);
	return text;
}

void Poly::update_text(Text &text, const Message::Text &new_config) {
	text.set_text(new_config.content);
}
