//
// Created by kenym on 08/02/24.
//

#include "text.hxx"

std::unique_ptr<Poly::Text> Poly::make_text(const Message::Text &msg) {
	auto text = std::make_unique<Text>();
	if (msg.tag.has_value()) {
		text->tag = *msg.tag;
	}
	text->set_placeholder_text(msg.content);
	return text;
}