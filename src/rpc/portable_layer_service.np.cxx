#include <exception>
#include <nanopack/reader.hxx>
#include <nanopack/writer.hxx>
#include <string>

#include "portable_layer_service.np.hxx"

Rpc::PortableLayerServiceServer::PortableLayerServiceServer()
    : NanoPack::RpcServer(), handlers() {
  handlers.reserve(2);
  handlers.emplace("invoke_void_callback",
                   &PortableLayerServiceServer::_invoke_void_callback);
  handlers.emplace("invoke_callback",
                   &PortableLayerServiceServer::_invoke_callback);
}

NanoPack::RpcServer::MethodCallResult
Rpc::PortableLayerServiceServer::on_method_call(const std::string_view &method,
                                                uint8_t *request_data,
                                                size_t offset,
                                                NanoPack::MessageId msg_id) {
  const auto handler = handlers.find(method);
  if (handler == handlers.end()) {
    throw std::invalid_argument("Unknown method " + std::string(method) +
                                " called on PortableLayerService.");
  }
  return (this->*(handler->second))(request_data, offset, msg_id);
}
NanoPack::RpcServer::MethodCallResult
Rpc::PortableLayerServiceServer::_invoke_void_callback(
    uint8_t *request_data, size_t offset, NanoPack::MessageId msg_id) {
  NanoPack::Reader reader(request_data);
  size_t ptr = offset;
  uint8_t *buf = reader.buffer;
  uint32_t handle;
  reader.read_uint32(ptr, handle);
  ptr += 4;
  std::unique_ptr<NanoPack::Message> args;
  size_t args_bytes_read;
  reader.buffer += ptr;
  args = std::move(make_nanopack_message(reader, args_bytes_read));
  reader.buffer = buf;
  ptr += args_bytes_read;

  invoke_void_callback(handle, std::move(args));

  NanoPack::Writer writer(6);
  writer.append_uint8(NanoPack::RpcMessageType::Response);
  writer.append_uint32(msg_id);
  writer.append_uint8(0);

  return {writer.into_data(), writer.size()};
}

NanoPack::RpcServer::MethodCallResult
Rpc::PortableLayerServiceServer::_invoke_callback(uint8_t *request_data,
                                                  size_t offset,
                                                  NanoPack::MessageId msg_id) {
  NanoPack::Reader reader(request_data);
  size_t ptr = offset;
  uint8_t *buf = reader.buffer;
  uint32_t handle;
  reader.read_uint32(ptr, handle);
  ptr += 4;
  std::unique_ptr<NanoPack::Message> args;
  size_t args_bytes_read;
  reader.buffer += ptr;
  args = std::move(make_nanopack_message(reader, args_bytes_read));
  reader.buffer = buf;
  ptr += args_bytes_read;

  std::unique_ptr<NanoPack::Message> result =
      std::move(invoke_callback(handle, std::move(args)));
  NanoPack::Writer writer(6);
  writer.append_uint8(NanoPack::RpcMessageType::Response);
  writer.append_uint32(msg_id);
  writer.append_uint8(0);
  const size_t result_byte_size = result->write_to(writer, writer.size());
  return {writer.into_data(), writer.size()};
}

std::future<void> Rpc::PortableLayerServiceClient::invoke_void_callback(
    uint32_t handle, std::unique_ptr<NanoPack::Message> args) {
  NanoPack::Writer writer(9 + 20);
  const auto msg_id = new_message_id();
  writer.append_uint8(NanoPack::RpcMessageType::Request);
  writer.append_uint32(msg_id);
  writer.append_uint32(20);
  writer.append_string_view("invoke_void_callback");
  writer.append_uint32(handle);
  const size_t args_byte_size = args->write_to(writer, writer.size());

  return std::async(
      [this](uint32_t msg_id, uint8_t *req_data, size_t req_size) {
        auto res_data =
            send_request_data_async(msg_id, req_data, req_size).get();
        NanoPack::Reader reader(res_data);
        size_t ptr = 0;
        uint8_t err_flag;
        reader.read_uint8(ptr++, err_flag);
        if (err_flag == 1) {
          throw std::runtime_error(
              "RPC on PortableLayer::invoke_void_callback failed.");
        }
        free(req_data);
      },
      msg_id, writer.into_data(), writer.size());
}

std::future<std::unique_ptr<NanoPack::Message>>
Rpc::PortableLayerServiceClient::invoke_callback(
    uint32_t handle, std::unique_ptr<NanoPack::Message> args) {
  NanoPack::Writer writer(9 + 15);
  const auto msg_id = new_message_id();
  writer.append_uint8(NanoPack::RpcMessageType::Request);
  writer.append_uint32(msg_id);
  writer.append_uint32(15);
  writer.append_string_view("invoke_callback");
  writer.append_uint32(handle);
  const size_t args_byte_size = args->write_to(writer, writer.size());

  return std::async(
      [this](uint32_t msg_id, uint8_t *req_data, size_t req_size) {
        auto res_data =
            send_request_data_async(msg_id, req_data, req_size).get();
        NanoPack::Reader reader(res_data);
        size_t ptr = 0;
        uint8_t err_flag;
        reader.read_uint8(ptr++, err_flag);
        if (err_flag == 1) {
          throw std::runtime_error(
              "RPC on PortableLayer::invoke_callback failed.");
        }
        free(req_data);

        uint8_t *buf = reader.buffer;
        std::unique_ptr<NanoPack::Message> result;
        size_t result_bytes_read;
        reader.buffer += ptr;
        result = std::move(make_nanopack_message(reader, result_bytes_read));
        reader.buffer = buf;
        ptr += result_bytes_read;
        return result;
      },
      msg_id, writer.into_data(), writer.size());
}
