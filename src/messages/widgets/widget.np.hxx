// AUTOMATICALLY GENERATED BY NANOPACK. DO NOT MODIFY BY HAND.

#ifndef WIDGET_NP_HXX
#define WIDGET_NP_HXX

#include <nanopack/message.hxx>
#include <nanopack/reader.hxx>
#include <optional>
#include <vector>

namespace Poly::Message {

struct Widget : NanoPack::Message {
  static constexpr int32_t TYPE_ID = 100;

  std::optional<int32_t> tag;

  Widget() = default;

  explicit Widget(std::optional<int32_t> tag);

  Widget(std::vector<uint8_t>::const_iterator begin, int &bytes_read);

  Widget(const NanoPack::Reader &reader, int &bytes_read);

  [[nodiscard]] int32_t type_id() const override;

  [[nodiscard]] std::vector<uint8_t> data() const override;
};

} // namespace Poly::Message

#endif
