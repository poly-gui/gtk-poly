// AUTOMATICALLY GENERATED BY NANOPACK. DO NOT MODIFY BY HAND.

#ifndef LIST_VIEW_NP_HXX
#define LIST_VIEW_NP_HXX

#include <nanopack/nanopack.hxx>
#include <nanopack/reader.hxx>
#include <vector>

#include "../make_widget.np.hxx"
#include "../widget.np.hxx"

namespace Poly::Message {

struct ListView : Widget {
  static constexpr NanoPack::TypeId TYPE_ID = 2164488861;

  double width;
  double height;
  std::vector<uint32_t> sections;
  double item_height;
  int32_t on_create;
  int32_t on_bind;

  ListView() = default;

  ListView(std::optional<int32_t> tag, double width, double height,
           std::vector<uint32_t> sections, double item_height,
           int32_t on_create, int32_t on_bind);

  ListView(std::vector<uint8_t>::const_iterator begin, int &bytes_read);

  ListView(const NanoPack::Reader &reader, int &bytes_read);

  size_t write_to(std::vector<uint8_t> &buf, int offset) const override;

  [[nodiscard]] NanoPack::TypeId type_id() const override;

  [[nodiscard]] int header_size() const override;

  [[nodiscard]] std::vector<uint8_t> data() const override;
};

} // namespace Poly::Message

#endif
