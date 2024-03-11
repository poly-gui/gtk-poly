// AUTOMATICALLY GENERATED BY NANOPACK. DO NOT MODIFY BY HAND.

#include <nanopack/reader.hxx>
#include <nanopack/writer.hxx>

#include "on_value_changed.np.hxx"

Poly::Message::OnValueChanged::OnValueChanged(std::string new_value)
    : new_value(std::move(new_value)) {}

Poly::Message::OnValueChanged::OnValueChanged(const NanoPack::Reader &reader,
                                              int &bytes_read) {
  const auto begin = reader.begin();
  int ptr = 8;

  const int32_t new_value_size = reader.read_field_size(0);
  new_value = reader.read_string(ptr, new_value_size);
  ptr += new_value_size;

  bytes_read = ptr;
}

Poly::Message::OnValueChanged::OnValueChanged(
    std::vector<uint8_t>::const_iterator begin, int &bytes_read)
    : OnValueChanged(NanoPack::Reader(begin), bytes_read) {}

NanoPack::TypeId Poly::Message::OnValueChanged::type_id() const {
  return TYPE_ID;
}

int Poly::Message::OnValueChanged::header_size() const { return 8; }

size_t Poly::Message::OnValueChanged::write_to(std::vector<uint8_t> &buf,
                                               int offset) const {
  size_t bytes_written = 8;

  buf.resize(offset + 8);

  NanoPack::write_type_id(TYPE_ID, offset, buf);

  NanoPack::write_field_size(0, new_value.size(), offset, buf);
  NanoPack::append_string(new_value, buf);
  bytes_written += new_value.size();

  return bytes_written;
}

std::vector<uint8_t> Poly::Message::OnValueChanged::data() const {
  std::vector<uint8_t> buf(8);
  write_to(buf, 0);
  return buf;
}
