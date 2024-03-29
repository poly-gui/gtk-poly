// AUTOMATICALLY GENERATED BY NANOPACK. DO NOT MODIFY BY HAND.

#include <nanopack/reader.hxx>
#include <nanopack/writer.hxx>

#include "request.np.hxx"

Poly::Message::Request::Request(uint32_t id, NanoPack::Any body)
    : id(id), body(std::move(body)) {}

Poly::Message::Request::Request(const NanoPack::Reader &reader,
                                int &bytes_read) {
  const auto begin = reader.begin();
  int ptr = 12;

  const uint32_t id = reader.read_uint32(ptr);
  ptr += 4;
  this->id = id;

  const int32_t body_byte_size = reader.read_field_size(1);
  body = NanoPack::Any(begin + ptr, begin + ptr + body_byte_size);
  ptr += body_byte_size;

  bytes_read = ptr;
}

Poly::Message::Request::Request(std::vector<uint8_t>::const_iterator begin,
                                int &bytes_read)
    : Request(NanoPack::Reader(begin), bytes_read) {}

NanoPack::TypeId Poly::Message::Request::type_id() const { return TYPE_ID; }

int Poly::Message::Request::header_size() const { return 12; }

size_t Poly::Message::Request::write_to(std::vector<uint8_t> &buf,
                                        int offset) const {
  size_t bytes_written = 12;

  buf.resize(offset + 12);

  NanoPack::write_type_id(TYPE_ID, offset, buf);

  NanoPack::write_field_size(0, 4, offset, buf);
  NanoPack::append_uint32(id, buf);
  bytes_written += 4;

  NanoPack::write_field_size(1, body.size(), offset, buf);
  NanoPack::append_bytes(body.data(), buf);
  bytes_written += body.size();

  return bytes_written;
}

std::vector<uint8_t> Poly::Message::Request::data() const {
  std::vector<uint8_t> buf(12);
  write_to(buf, 0);
  return buf;
}
