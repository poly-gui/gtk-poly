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

int Poly::Message::ListViewItem::header_size() const { return 12; }

size_t Poly::Message::ListViewItem::write_to(std::vector<uint8_t> &buf,
                                             int offset) const {
  size_t bytes_written = 12;

  buf.resize(offset + 12);

  NanoPack::write_type_id(TYPE_ID, offset, buf);

  NanoPack::write_field_size(0, 4, offset, buf);
  NanoPack::append_uint32(item_tag, buf);
  bytes_written += 4;

  const size_t widget_byte_size = widget->write_to(buf, buf.size());
  NanoPack::write_field_size(1, widget_byte_size, offset, buf);

  return bytes_written;
}

std::vector<uint8_t> Poly::Message::ListViewItem::data() const {
  std::vector<uint8_t> buf(12);
  write_to(buf, 0);
  return buf;
}