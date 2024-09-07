#ifndef PORTABLE_LAYER_SERVICE_NP_HXX
#define PORTABLE_LAYER_SERVICE_NP_HXX

#include "nanopack_message_factory.np.hxx"
#include <future>
#include <memory>
#include <nanopack/message.hxx>
#include <nanopack/rpc.hxx>
#include <string_view>
#include <unordered_map>

namespace Rpc {

class PortableLayerServiceServer : public NanoPack::RpcServer {
  std::unordered_map<std::string_view,
                     MethodCallResult (PortableLayerServiceServer::*)(
                         uint8_t *, size_t, NanoPack::MessageId)>
      handlers;

  MethodCallResult on_method_call(const std::string_view &method,
                                  uint8_t *request_data, size_t offset,
                                  NanoPack::MessageId msg_id) override;
  MethodCallResult _invoke_void_callback(uint8_t *request_data, size_t offset,
                                         NanoPack::MessageId msg_id);
  virtual void
  invoke_void_callback(uint32_t handle,
                       std::unique_ptr<NanoPack::Message> args) = 0;

  MethodCallResult _invoke_callback(uint8_t *request_data, size_t offset,
                                    NanoPack::MessageId msg_id);
  virtual std::unique_ptr<NanoPack::Message>
  invoke_callback(uint32_t handle, std::unique_ptr<NanoPack::Message> args) = 0;

public:
  PortableLayerServiceServer();
};

class PortableLayerServiceClient : public NanoPack::RpcClient {
public:
  using NanoPack::RpcClient::RpcClient;
  std::future<void>
  invoke_void_callback(uint32_t handle,
                       std::unique_ptr<NanoPack::Message> args);

  std::future<std::unique_ptr<NanoPack::Message>>
  invoke_callback(uint32_t handle, std::unique_ptr<NanoPack::Message> args);
};

} // namespace Rpc

#endif
