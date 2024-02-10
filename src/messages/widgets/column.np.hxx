// AUTOMATICALLY GENERATED BY NANOPACK. DO NOT MODIFY BY HAND.

#ifndef COLUMN_NP_HXX
#define COLUMN_NP_HXX

#include <nanopack/reader.hxx>
#include <vector>

#include "alignment.np.hxx"
#include "make_widget.np.hxx"
#include "widget.np.hxx"

namespace Poly::Message {

struct Column : Widget {
  static constexpr int32_t TYPE_ID = 103;

  double width;
  double height;
  Alignment horizontal_alignment;
  Alignment vertical_alignment;
  std::vector<std::unique_ptr<Widget>> children;

  Column() = default;

  Column(std::optional<int32_t> tag, double width, double height,
         const Alignment &horizontal_alignment,
         const Alignment &vertical_alignment,
         std::vector<std::unique_ptr<Widget>> children);

  Column(std::vector<uint8_t>::const_iterator begin, int &bytes_read);

  Column(const NanoPack::Reader &reader, int &bytes_read);

  [[nodiscard]] int32_t type_id() const override;

  [[nodiscard]] std::vector<uint8_t> data() const override;
};

} // namespace Poly::Message

#endif
