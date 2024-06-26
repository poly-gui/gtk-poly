// AUTOMATICALLY GENERATED BY NANOPACK. DO NOT MODIFY BY HAND.

#include <nanopack/reader.hxx>
#include <nanopack/writer.hxx>

#include "font_style.np.hxx"

Poly::Message::FontStyle::FontStyle(std::string font_family,
                                    uint32_t font_weight, uint32_t font_size)
    : font_family(std::move(font_family)), font_weight(font_weight),
      font_size(font_size) {}

Poly::Message::FontStyle::FontStyle(const NanoPack::Reader &reader,
                                    int &bytes_read) {
  const auto begin = reader.begin();
  int ptr = 16;

  const int32_t font_family_size = reader.read_field_size(0);
  font_family = reader.read_string(ptr, font_family_size);
  ptr += font_family_size;

  const uint32_t font_weight = reader.read_uint32(ptr);
  ptr += 4;
  this->font_weight = font_weight;

  const uint32_t font_size = reader.read_uint32(ptr);
  ptr += 4;
  this->font_size = font_size;

  bytes_read = ptr;
}

Poly::Message::FontStyle::FontStyle(std::vector<uint8_t>::const_iterator begin,
                                    int &bytes_read)
    : FontStyle(NanoPack::Reader(begin), bytes_read) {}

NanoPack::TypeId Poly::Message::FontStyle::type_id() const { return TYPE_ID; }

int Poly::Message::FontStyle::header_size() const { return 16; }

size_t Poly::Message::FontStyle::write_to(std::vector<uint8_t> &buf,
                                          int offset) const {
  size_t bytes_written = 16;

  buf.resize(offset + 16);

  NanoPack::write_type_id(TYPE_ID, offset, buf);

  NanoPack::write_field_size(0, font_family.size(), offset, buf);
  NanoPack::append_string(font_family, buf);
  bytes_written += font_family.size();

  NanoPack::write_field_size(1, 4, offset, buf);
  NanoPack::append_uint32(font_weight, buf);
  bytes_written += 4;

  NanoPack::write_field_size(2, 4, offset, buf);
  NanoPack::append_uint32(font_size, buf);
  bytes_written += 4;

  return bytes_written;
}

std::vector<uint8_t> Poly::Message::FontStyle::data() const {
  std::vector<uint8_t> buf(16);
  write_to(buf, 0);
  return buf;
}
