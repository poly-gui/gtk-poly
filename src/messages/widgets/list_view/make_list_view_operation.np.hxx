// AUTOMATICALLY GENERATED BY NANOPACK. DO NOT MODIFY BY HAND.

#ifndef LIST_VIEW_OPERATION_FACTORY_NP_HXX
#define LIST_VIEW_OPERATION_FACTORY_NP_HXX

#include "list_view_operation.np.hxx"
#include <memory>

namespace Poly::Message {

std::unique_ptr<ListViewOperation>
make_list_view_operation(std::vector<uint8_t>::const_iterator begin,
                         int &bytes_read);

} // namespace Poly::Message

#endif