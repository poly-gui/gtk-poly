// AUTOMATICALLY GENERATED BY NANOPACK. DO NOT MODIFY BY HAND.

#ifndef INVOKE_CALLBACK_NP_HXX
#define INVOKE_CALLBACK_NP_HXX

#include <nanopack/any.hxx>
#include <nanopack/message.hxx>
#include <nanopack/nanopack.hxx>
#include <nanopack/reader.hxx>
#include <vector>

namespace Poly::Message {

struct InvokeCallback : NanoPack::Message {
  static constexpr NanoPack::TypeId TYPE_ID = 2013877267;

  int32_t handle;
  NanoPack::Any args;

  InvokeCallback() = default;

  InvokeCallback(int32_t handle, NanoPack::Any args);

  InvokeCallback(std::vector<uint8_t>::const_iterator begin, int &bytes_read);

  InvokeCallback(const NanoPack::Reader &reader, int &bytes_read);

  [[nodiscard]] NanoPack::TypeId type_id() const override;

  [[nodiscard]] std::vector<uint8_t> data() const override;

  [[nodiscard]] std::vector<uint8_t> data_with_length_prefix() const override;
};

} // namespace Poly::Message

#endif
