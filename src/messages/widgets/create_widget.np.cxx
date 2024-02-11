// AUTOMATICALLY GENERATED BY NANOPACK. DO NOT MODIFY BY HAND.

#include <nanopack/reader.hxx>
#include <nanopack/writer.hxx>

#include "create_widget.np.hxx"

Poly::Message::CreateWidget::CreateWidget(std::unique_ptr<Widget> widget,
                                          std::string window_tag)
    : widget(std::move(widget)), window_tag(std::move(window_tag)) {}

Poly::Message::CreateWidget::CreateWidget(const NanoPack::Reader &reader,
                                          int &bytes_read) {
  const auto begin = reader.begin();
  int ptr = 12;

  int widget_bytes_read = 0;
  widget = std::move(make_widget(begin + ptr, widget_bytes_read));
  ptr += widget_bytes_read;

  const int32_t window_tag_size = reader.read_field_size(1);
  window_tag = reader.read_string(ptr, window_tag_size);
  ptr += window_tag_size;

  bytes_read = ptr;
}

Poly::Message::CreateWidget::CreateWidget(
    std::vector<uint8_t>::const_iterator begin, int &bytes_read)
    : CreateWidget(NanoPack::Reader(begin), bytes_read) {}

Poly::Message::Widget &Poly::Message::CreateWidget::get_widget() const {
  return *widget;
}

int32_t Poly::Message::CreateWidget::type_id() const { return TYPE_ID; }

std::vector<uint8_t> Poly::Message::CreateWidget::data() const {
  std::vector<uint8_t> buf(12);
  NanoPack::Writer writer(&buf);

  writer.write_type_id(TYPE_ID);

  const std::vector<uint8_t> widget_data = widget->data();
  writer.append_bytes(widget_data);
  writer.write_field_size(0, widget_data.size());

  writer.write_field_size(1, window_tag.size());
  writer.append_string(window_tag);

  return buf;
}

std::vector<uint8_t>
Poly::Message::CreateWidget::data_with_length_prefix() const {
  std::vector<uint8_t> buf(12 + 4);
  NanoPack::Writer writer(&buf, 4);

  writer.write_type_id(TYPE_ID);

  const std::vector<uint8_t> widget_data = widget->data();
  writer.append_bytes(widget_data);
  writer.write_field_size(0, widget_data.size());

  writer.write_field_size(1, window_tag.size());
  writer.append_string(window_tag);

  const size_t byte_size = buf.size() - 4;
  buf[0] = byte_size & 0xFF;
  buf[1] = byte_size & 0xFF00;
  buf[2] = byte_size & 0xFF0000;
  buf[3] = byte_size & 0xFF000000;

  return buf;
}
