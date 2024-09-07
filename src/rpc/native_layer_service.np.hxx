#ifndef NATIVE_LAYER_SERVICE_NP_HXX
#define NATIVE_LAYER_SERVICE_NP_HXX

#include "nanopack_message_factory.np.hxx"
#include "widget/make_widget.np.hxx"
#include "widget/widget.np.hxx"
#include <future>
#include <memory>
#include <nanopack/message.hxx>
#include <nanopack/rpc.hxx>
#include <string_view>
#include <unordered_map>

namespace Rpc {

class NativeLayerServiceServer : public NanoPack::RpcServer {
  std::unordered_map<std::string_view,
                     MethodCallResult (NativeLayerServiceServer::*)(
                         uint8_t *, size_t, NanoPack::MessageId)>
      handlers;

  MethodCallResult on_method_call(const std::string_view &method,
                                  uint8_t *request_data, size_t offset,
                                  NanoPack::MessageId msg_id) override;
  MethodCallResult _create_window(uint8_t *request_data, size_t offset,
                                  NanoPack::MessageId msg_id);
  virtual void create_window(const std::string &title,
                             const std::string &description, int32_t width,
                             int32_t height, const std::string &tag) = 0;

  MethodCallResult _create_widget(uint8_t *request_data, size_t offset,
                                  NanoPack::MessageId msg_id);
  virtual void create_widget(std::unique_ptr<Widget> widget,
                             const std::string &window_tag) = 0;

  MethodCallResult _append_new_widget(uint8_t *request_data, size_t offset,
                                      NanoPack::MessageId msg_id);
  virtual void append_new_widget(std::unique_ptr<Widget> child,
                                 uint32_t parent_tag) = 0;

  MethodCallResult _update_widget(uint8_t *request_data, size_t offset,
                                  NanoPack::MessageId msg_id);
  virtual void update_widget(uint32_t tag, std::unique_ptr<Widget> widget,
                             std::unique_ptr<NanoPack::Message> args) = 0;

  MethodCallResult _update_widgets(uint8_t *request_data, size_t offset,
                                   NanoPack::MessageId msg_id);
  virtual void
  update_widgets(const std::vector<uint32_t> &tag,
                 const std::vector<std::unique_ptr<Widget>> &widgets,
                 std::unique_ptr<NanoPack::Message> args) = 0;

  MethodCallResult _remove_widget(uint8_t *request_data, size_t offset,
                                  NanoPack::MessageId msg_id);
  virtual void remove_widget(uint32_t tag) = 0;

  MethodCallResult _insert_widget_before(uint8_t *request_data, size_t offset,
                                         NanoPack::MessageId msg_id);
  virtual void insert_widget_before(std::unique_ptr<Widget> widget,
                                    std::unique_ptr<Widget> before_widget,
                                    uint32_t parent_tag) = 0;

  MethodCallResult _clear_window(uint8_t *request_data, size_t offset,
                                 NanoPack::MessageId msg_id);
  virtual void clear_window(const std::string &window_tag) = 0;

public:
  NativeLayerServiceServer();
};

class NativeLayerServiceClient : public NanoPack::RpcClient {
public:
  using NanoPack::RpcClient::RpcClient;
  std::future<void> create_window(const std::string &title,
                                  const std::string &description, int32_t width,
                                  int32_t height, const std::string &tag);

  std::future<void> create_widget(std::unique_ptr<Widget> widget,
                                  const std::string &window_tag);

  std::future<void> append_new_widget(std::unique_ptr<Widget> child,
                                      uint32_t parent_tag);

  std::future<void> update_widget(uint32_t tag, std::unique_ptr<Widget> widget,
                                  std::unique_ptr<NanoPack::Message> args);

  std::future<void>
  update_widgets(const std::vector<uint32_t> &tag,
                 const std::vector<std::unique_ptr<Widget>> &widgets,
                 std::unique_ptr<NanoPack::Message> args);

  std::future<void> remove_widget(uint32_t tag);

  std::future<void> insert_widget_before(std::unique_ptr<Widget> widget,
                                         std::unique_ptr<Widget> before_widget,
                                         uint32_t parent_tag);

  std::future<void> clear_window(const std::string &window_tag);
};

} // namespace Rpc

#endif
