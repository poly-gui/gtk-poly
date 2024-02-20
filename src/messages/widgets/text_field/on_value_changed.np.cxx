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

int32_t Poly::Message::OnValueChanged::type_id() const { return TYPE_ID; }

std::vector<uint8_t> Poly::Message::OnValueChanged::data() const {
  std::vector<uint8_t> buf(8);
  NanoPack::Writer writer(&buf);

  writer.write_type_id(TYPE_ID);

  writer.write_field_size(0, new_value.size());
  writer.append_string(new_value);

  return buf;
}

std::vector<uint8_t>
Poly::Message::OnValueChanged::data_with_length_prefix() const {
  std::vector<uint8_t> buf(8 + 4);
  NanoPack::Writer writer(&buf, 4);

  writer.write_type_id(TYPE_ID);

  writer.write_field_size(0, new_value.size());
  writer.append_string(new_value);

  const size_t byte_size = buf.size() - 4;
  buf[0] = byte_size & 0xFF;
  buf[1] = byte_size & 0xFF00;
  buf[2] = byte_size & 0xFF0000;
  buf[3] = byte_size & 0xFF000000;

  return buf;
}
