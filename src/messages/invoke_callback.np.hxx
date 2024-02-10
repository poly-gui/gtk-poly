// AUTOMATICALLY GENERATED BY NANOPACK. DO NOT MODIFY BY HAND.

#ifndef INVOKE_CALLBACK_NP_HXX
#define INVOKE_CALLBACK_NP_HXX

#include <nanopack/any.hxx>
#include <nanopack/message.hxx>
#include <nanopack/reader.hxx>
#include <vector>

namespace Poly::Message {

struct InvokeCallback : NanoPack::Message {
  static constexpr int32_t TYPE_ID = 2;

  int32_t handle;
  NanoPack::Any args;

  InvokeCallback() = default;

  InvokeCallback(int32_t handle, NanoPack::Any args);

  InvokeCallback(std::vector<uint8_t>::const_iterator begin, int &bytes_read);

  InvokeCallback(const NanoPack::Reader &reader, int &bytes_read);

  [[nodiscard]] int32_t type_id() const override;

  [[nodiscard]] std::vector<uint8_t> data() const override;
};

} // namespace Poly::Message

#endif
