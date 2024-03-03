// AUTOMATICALLY GENERATED BY NANOPACK. DO NOT MODIFY BY HAND.

#ifndef UPDATE_WIDGET_NP_HXX
#define UPDATE_WIDGET_NP_HXX

#include <memory>
#include <nanopack/any.hxx>
#include <nanopack/message.hxx>
#include <nanopack/nanopack.hxx>
#include <nanopack/reader.hxx>
#include <optional>
#include <vector>

#include "make_widget.np.hxx"
#include "widget.np.hxx"

namespace Poly::Message {

struct UpdateWidget : NanoPack::Message {
  static constexpr NanoPack::TypeId TYPE_ID = 1016534798;

  int32_t tag;
  std::unique_ptr<Widget> widget;
  std::optional<NanoPack::Any> args;

  UpdateWidget() = default;

  UpdateWidget(int32_t tag, std::unique_ptr<Widget> widget,
               std::optional<NanoPack::Any> args);

  UpdateWidget(std::vector<uint8_t>::const_iterator begin, int &bytes_read);

  UpdateWidget(const NanoPack::Reader &reader, int &bytes_read);

  [[nodiscard]] Widget &get_widget() const;

  [[nodiscard]] NanoPack::TypeId type_id() const override;

  [[nodiscard]] std::vector<uint8_t> data() const override;

  [[nodiscard]] std::vector<uint8_t> data_with_length_prefix() const override;
};

} // namespace Poly::Message

#endif
