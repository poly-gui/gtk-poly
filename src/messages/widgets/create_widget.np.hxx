// AUTOMATICALLY GENERATED BY NANOPACK. DO NOT MODIFY BY HAND.

#ifndef CREATE_WIDGET_NP_HXX
#define CREATE_WIDGET_NP_HXX

#include <memory>
#include <nanopack/message.hxx>
#include <nanopack/nanopack.hxx>
#include <nanopack/reader.hxx>
#include <string>
#include <vector>

#include "make_widget.np.hxx"
#include "widget.np.hxx"

namespace Poly::Message {

struct CreateWidget : NanoPack::Message {
  static constexpr NanoPack::TypeId TYPE_ID = 2313387354;

  std::unique_ptr<Widget> widget;
  std::string window_tag;

  CreateWidget() = default;

  CreateWidget(std::unique_ptr<Widget> widget, std::string window_tag);

  CreateWidget(std::vector<uint8_t>::const_iterator begin, int &bytes_read);

  CreateWidget(const NanoPack::Reader &reader, int &bytes_read);

  [[nodiscard]] Widget &get_widget() const;

  size_t write_to(std::vector<uint8_t> &buf, int offset) const override;

  [[nodiscard]] NanoPack::TypeId type_id() const override;

  [[nodiscard]] int header_size() const override;

  [[nodiscard]] std::vector<uint8_t> data() const override;
};

} // namespace Poly::Message

#endif
