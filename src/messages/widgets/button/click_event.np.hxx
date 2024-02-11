// AUTOMATICALLY GENERATED BY NANOPACK. DO NOT MODIFY BY HAND.

#ifndef CLICK_EVENT_NP_HXX
#define CLICK_EVENT_NP_HXX

#include <nanopack/message.hxx>
#include <nanopack/reader.hxx>
#include <vector>

namespace Poly::Message {

struct ClickEvent : NanoPack::Message {
  static constexpr int32_t TYPE_ID = 1041;

  int32_t timestamp;

  ClickEvent() = default;

  explicit ClickEvent(int32_t timestamp);

  ClickEvent(std::vector<uint8_t>::const_iterator begin, int &bytes_read);

  ClickEvent(const NanoPack::Reader &reader, int &bytes_read);

  [[nodiscard]] int32_t type_id() const override;

  [[nodiscard]] std::vector<uint8_t> data() const override;

  [[nodiscard]] std::vector<uint8_t> data_with_length_prefix() const override;
};

} // namespace Poly::Message

#endif
