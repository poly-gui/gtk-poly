// AUTOMATICALLY GENERATED BY NANOPACK. DO NOT MODIFY BY HAND.

#include <nanopack/reader.hxx>
#include <nanopack/writer.hxx>

#include "list_view.np.hxx"

Poly::Message::ListView::ListView(std::optional<int32_t> tag, double width,
                                  double height, std::vector<uint32_t> sections,
                                  double item_height, int32_t on_create,
                                  int32_t on_bind)
    : Widget(tag), width(width), height(height), sections(std::move(sections)),
      item_height(item_height), on_create(on_create), on_bind(on_bind) {}

Poly::Message::ListView::ListView(const NanoPack::Reader &reader,
                                  int &bytes_read)
    : Widget() {
  const auto begin = reader.begin();
  int ptr = 32;

  if (reader.read_field_size(0) < 0) {
    this->tag = std::nullopt;
  } else {
    const int32_t tag = reader.read_int32(ptr);
    ptr += 4;
    this->tag = tag;
  }

  const double width = reader.read_double(ptr);
  ptr += 8;
  this->width = width;

  const double height = reader.read_double(ptr);
  ptr += 8;
  this->height = height;

  const int32_t sections_byte_size = reader.read_field_size(3);
  const int32_t sections_vec_size = sections_byte_size / 4;
  std::vector<uint32_t> sections;
  sections.reserve(sections_vec_size);
  for (int i = 0; i < sections_vec_size; i++) {
    const uint32_t i_item = reader.read_uint32(ptr);
    ptr += 4;
    sections.emplace_back(std::move(i_item));
  }
  this->sections = std::move(sections);

  const double item_height = reader.read_double(ptr);
  ptr += 8;
  this->item_height = item_height;

  const int32_t on_create = reader.read_int32(ptr);
  ptr += 4;
  this->on_create = on_create;

  const int32_t on_bind = reader.read_int32(ptr);
  ptr += 4;
  this->on_bind = on_bind;

  bytes_read = ptr;
}

Poly::Message::ListView::ListView(std::vector<uint8_t>::const_iterator begin,
                                  int &bytes_read)
    : ListView(NanoPack::Reader(begin), bytes_read) {}

NanoPack::TypeId Poly::Message::ListView::type_id() const { return TYPE_ID; }

int Poly::Message::ListView::header_size() const { return 32; }

size_t Poly::Message::ListView::write_to(std::vector<uint8_t> &buf,
                                         int offset) const {
  size_t bytes_written = 32;

  buf.resize(offset + 32);

  NanoPack::write_type_id(TYPE_ID, offset, buf);

  if (tag.has_value()) {
    const auto tag = this->tag.value();
    NanoPack::write_field_size(0, 4, offset, buf);
    NanoPack::append_int32(tag, buf);
    bytes_written += 4;
  } else {
    NanoPack::write_field_size(0, -1, offset, buf);
  }

  NanoPack::write_field_size(1, 8, offset, buf);
  NanoPack::append_double(width, buf);
  bytes_written += 8;

  NanoPack::write_field_size(2, 8, offset, buf);
  NanoPack::append_double(height, buf);
  bytes_written += 8;

  const int32_t sections_byte_size = sections.size() * 4;
  NanoPack::write_field_size(3, sections_byte_size, offset, buf);
  for (const auto &i : sections) {
    NanoPack::append_uint32(i, buf);
  }
  bytes_written += sections_byte_size;

  NanoPack::write_field_size(4, 8, offset, buf);
  NanoPack::append_double(item_height, buf);
  bytes_written += 8;

  NanoPack::write_field_size(5, 4, offset, buf);
  NanoPack::append_int32(on_create, buf);
  bytes_written += 4;

  NanoPack::write_field_size(6, 4, offset, buf);
  NanoPack::append_int32(on_bind, buf);
  bytes_written += 4;

  return bytes_written;
}

std::vector<uint8_t> Poly::Message::ListView::data() const {
  std::vector<uint8_t> buf(32);
  write_to(buf, 0);
  return buf;
}
