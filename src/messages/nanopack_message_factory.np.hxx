// AUTOMATICALLY GENERATED BY NANOPACK. DO NOT MODIFY BY HAND.

#ifndef NANOPACK_MESSAGE_FACTORY_HXX
#define NANOPACK_MESSAGE_FACTORY_HXX

#include <memory>
#include <nanopack/message.hxx>

namespace Poly::Message {

std::unique_ptr<NanoPack::Message>
make_nanopack_message(std::vector<uint8_t>::const_iterator data_iter,
                      int &bytes_read);

} // namespace Poly::Message

#endif
