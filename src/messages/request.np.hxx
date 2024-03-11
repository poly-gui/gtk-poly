// AUTOMATICALLY GENERATED BY NANOPACK. DO NOT MODIFY BY HAND.

#ifndef REQUEST_NP_HXX
#define REQUEST_NP_HXX

#include <nanopack/any.hxx>
#include <nanopack/message.hxx>
#include <nanopack/nanopack.hxx>
#include <nanopack/reader.hxx>
#include <vector>

namespace Poly::Message {

struct Request : NanoPack::Message {
  static constexpr NanoPack::TypeId TYPE_ID = 879254966;

  uint32_t id;
  NanoPack::Any body;

  Request() = default;

  Request(uint32_t id, NanoPack::Any body);

  Request(std::vector<uint8_t>::const_iterator begin, int &bytes_read);

  Request(const NanoPack::Reader &reader, int &bytes_read);

  size_t write_to(std::vector<uint8_t> &buf, int offset) const override;

  [[nodiscard]] NanoPack::TypeId type_id() const override;

  [[nodiscard]] int header_size() const override;

  [[nodiscard]] std::vector<uint8_t> data() const override;
};

} // namespace Poly::Message

#endif
