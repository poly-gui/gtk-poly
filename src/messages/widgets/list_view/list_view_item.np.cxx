// AUTOMATICALLY GENERATED BY NANOPACK. DO NOT MODIFY BY HAND.

#include <nanopack/reader.hxx>
#include <nanopack/writer.hxx>

#include "list_view_item.np.hxx"

Poly::Message::ListViewItem::ListViewItem(uint32_t item_tag,
                                          std::unique_ptr<Widget> widget)
    : item_tag(item_tag), widget(std::move(widget)) {}

Poly::Message::ListViewItem::ListViewItem(const NanoPack::Reader &reader,
                                          int &bytes_read) {
  const auto begin = reader.begin();
  int ptr = 12;

  const uint32_t item_tag = reader.read_uint32(ptr);
  ptr += 4;
  this->item_tag = item_tag;

  int widget_bytes_read = 0;
  widget = std::move(make_widget(begin + ptr, widget_bytes_read));
  ptr += widget_bytes_read;

  bytes_read = ptr;
}

Poly::Message::ListViewItem::ListViewItem(
    std::vector<uint8_t>::const_iterator begin, int &bytes_read)
    : ListViewItem(NanoPack::Reader(begin), bytes_read) {}

Poly::Message::Widget &Poly::Message::ListViewItem::get_widget() const {
  return *widget;
}

NanoPack::TypeId Poly::Message::ListViewItem::type_id() const {
  return TYPE_ID;
}

std::vector<uint8_t> Poly::Message::ListViewItem::data() const {
  std::vector<uint8_t> buf(12);
  NanoPack::Writer writer(&buf);

  writer.write_type_id(TYPE_ID);

  writer.write_field_size(0, 4);
  writer.append_uint32(item_tag);

  const std::vector<uint8_t> widget_data = widget->data();
  writer.append_bytes(widget_data);
  writer.write_field_size(1, widget_data.size());

  return buf;
}

std::vector<uint8_t>
Poly::Message::ListViewItem::data_with_length_prefix() const {
  std::vector<uint8_t> buf(12 + 4);
  NanoPack::Writer writer(&buf, 4);

  writer.write_type_id(TYPE_ID);

  writer.write_field_size(0, 4);
  writer.append_uint32(item_tag);

  const std::vector<uint8_t> widget_data = widget->data();
  writer.append_bytes(widget_data);
  writer.write_field_size(1, widget_data.size());

  const size_t byte_size = buf.size() - 4;
  buf[0] = byte_size & 0xFF;
  buf[1] = byte_size & 0xFF00;
  buf[2] = byte_size & 0xFF0000;
  buf[3] = byte_size & 0xFF000000;

  return buf;
}
