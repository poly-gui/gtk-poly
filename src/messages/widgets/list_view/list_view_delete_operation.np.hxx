// AUTOMATICALLY GENERATED BY NANOPACK. DO NOT MODIFY BY HAND.

#ifndef LIST_VIEW_DELETE_OPERATION_NP_HXX
#define LIST_VIEW_DELETE_OPERATION_NP_HXX

#include <nanopack/nanopack.hxx>
#include <nanopack/reader.hxx>
#include <vector>

#include "list_view_operation.np.hxx"
#include "make_list_view_operation.np.hxx"

namespace Poly::Message {

struct ListViewDeleteOperation : ListViewOperation {
  static constexpr NanoPack::TypeId TYPE_ID = 2223513129;

  std::vector<int32_t> delete_at;

  ListViewDeleteOperation() = default;

  ListViewDeleteOperation(int32_t tag, std::vector<int32_t> delete_at);

  ListViewDeleteOperation(std::vector<uint8_t>::const_iterator begin,
                          int &bytes_read);

  ListViewDeleteOperation(const NanoPack::Reader &reader, int &bytes_read);

  size_t write_to(std::vector<uint8_t> &buf, int offset) const override;

  [[nodiscard]] NanoPack::TypeId type_id() const override;

  [[nodiscard]] int header_size() const override;

  [[nodiscard]] std::vector<uint8_t> data() const override;
};

} // namespace Poly::Message

#endif
