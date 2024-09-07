#include <exception>
#include <nanopack/reader.hxx>
#include <nanopack/writer.hxx>
#include <string>

#include "native_layer_service.np.hxx"

Rpc::NativeLayerServiceServer::NativeLayerServiceServer()
    : NanoPack::RpcServer(), handlers() {
  handlers.reserve(8);
  handlers.emplace("create_window", &NativeLayerServiceServer::_create_window);
  handlers.emplace("create_widget", &NativeLayerServiceServer::_create_widget);
  handlers.emplace("append_new_widget",
                   &NativeLayerServiceServer::_append_new_widget);
  handlers.emplace("update_widget", &NativeLayerServiceServer::_update_widget);
  handlers.emplace("update_widgets",
                   &NativeLayerServiceServer::_update_widgets);
  handlers.emplace("remove_widget", &NativeLayerServiceServer::_remove_widget);
  handlers.emplace("insert_widget_before",
                   &NativeLayerServiceServer::_insert_widget_before);
  handlers.emplace("clear_window", &NativeLayerServiceServer::_clear_window);
}

NanoPack::RpcServer::MethodCallResult
Rpc::NativeLayerServiceServer::on_method_call(const std::string_view &method,
                                              uint8_t *request_data,
                                              size_t offset,
                                              NanoPack::MessageId msg_id) {
  const auto handler = handlers.find(method);
  if (handler == handlers.end()) {
    throw std::invalid_argument("Unknown method " + std::string(method) +
                                " called on NativeLayerService.");
  }
  return (this->*(handler->second))(request_data, offset, msg_id);
}
NanoPack::RpcServer::MethodCallResult
Rpc::NativeLayerServiceServer::_create_window(uint8_t *request_data,
                                              size_t offset,
                                              NanoPack::MessageId msg_id) {
  NanoPack::Reader reader(request_data);
  size_t ptr = offset;
  uint8_t *buf = reader.buffer;
  uint32_t title_size;
  reader.read_uint32(ptr, title_size);
  ptr += 4;
  std::string title;
  reader.read_string(ptr, title_size, title);
  ptr += title_size;
  uint32_t description_size;
  reader.read_uint32(ptr, description_size);
  ptr += 4;
  std::string description;
  reader.read_string(ptr, description_size, description);
  ptr += description_size;
  int32_t width;
  reader.read_int32(ptr, width);
  ptr += 4;
  int32_t height;
  reader.read_int32(ptr, height);
  ptr += 4;
  uint32_t tag_size;
  reader.read_uint32(ptr, tag_size);
  ptr += 4;
  std::string tag;
  reader.read_string(ptr, tag_size, tag);
  ptr += tag_size;

  create_window(title, description, width, height, tag);

  NanoPack::Writer writer(6);
  writer.append_uint8(NanoPack::RpcMessageType::Response);
  writer.append_uint32(msg_id);
  writer.append_uint8(0);

  return {writer.into_data(), writer.size()};
}

NanoPack::RpcServer::MethodCallResult
Rpc::NativeLayerServiceServer::_create_widget(uint8_t *request_data,
                                              size_t offset,
                                              NanoPack::MessageId msg_id) {
  NanoPack::Reader reader(request_data);
  size_t ptr = offset;
  uint8_t *buf = reader.buffer;
  std::unique_ptr<Widget> widget;
  size_t widget_bytes_read;
  reader.buffer += ptr;
  widget = std::move(make_widget(reader, widget_bytes_read));
  reader.buffer = buf;
  ptr += widget_bytes_read;
  uint32_t window_tag_size;
  reader.read_uint32(ptr, window_tag_size);
  ptr += 4;
  std::string window_tag;
  reader.read_string(ptr, window_tag_size, window_tag);
  ptr += window_tag_size;

  create_widget(std::move(widget), window_tag);

  NanoPack::Writer writer(6);
  writer.append_uint8(NanoPack::RpcMessageType::Response);
  writer.append_uint32(msg_id);
  writer.append_uint8(0);

  return {writer.into_data(), writer.size()};
}

NanoPack::RpcServer::MethodCallResult
Rpc::NativeLayerServiceServer::_append_new_widget(uint8_t *request_data,
                                                  size_t offset,
                                                  NanoPack::MessageId msg_id) {
  NanoPack::Reader reader(request_data);
  size_t ptr = offset;
  uint8_t *buf = reader.buffer;
  std::unique_ptr<Widget> child;
  size_t child_bytes_read;
  reader.buffer += ptr;
  child = std::move(make_widget(reader, child_bytes_read));
  reader.buffer = buf;
  ptr += child_bytes_read;
  uint32_t parent_tag;
  reader.read_uint32(ptr, parent_tag);
  ptr += 4;

  append_new_widget(std::move(child), parent_tag);

  NanoPack::Writer writer(6);
  writer.append_uint8(NanoPack::RpcMessageType::Response);
  writer.append_uint32(msg_id);
  writer.append_uint8(0);

  return {writer.into_data(), writer.size()};
}

NanoPack::RpcServer::MethodCallResult
Rpc::NativeLayerServiceServer::_update_widget(uint8_t *request_data,
                                              size_t offset,
                                              NanoPack::MessageId msg_id) {
  NanoPack::Reader reader(request_data);
  size_t ptr = offset;
  uint8_t *buf = reader.buffer;
  uint32_t tag;
  reader.read_uint32(ptr, tag);
  ptr += 4;
  std::unique_ptr<Widget> widget;
  size_t widget_bytes_read;
  reader.buffer += ptr;
  widget = std::move(make_widget(reader, widget_bytes_read));
  reader.buffer = buf;
  ptr += widget_bytes_read;
  std::unique_ptr<NanoPack::Message> args = nullptr;
  if (reader.buffer[ptr++] != 0) {
    reader.buffer += ptr;
    size_t args_bytes_read;
    args = std::move(make_nanopack_message(reader, args_bytes_read));
    reader.buffer = buf;
    ptr += args_bytes_read;
  }

  update_widget(tag, std::move(widget), std::move(args));

  NanoPack::Writer writer(6);
  writer.append_uint8(NanoPack::RpcMessageType::Response);
  writer.append_uint32(msg_id);
  writer.append_uint8(0);

  return {writer.into_data(), writer.size()};
}

NanoPack::RpcServer::MethodCallResult
Rpc::NativeLayerServiceServer::_update_widgets(uint8_t *request_data,
                                               size_t offset,
                                               NanoPack::MessageId msg_id) {
  NanoPack::Reader reader(request_data);
  size_t ptr = offset;
  uint8_t *buf = reader.buffer;
  int32_t tag_vec_size;
  reader.read_int32(ptr, tag_vec_size);
  ptr += 4;
  std::vector<uint32_t> tag(tag_vec_size);
  for (int i = 0; i < tag_vec_size; ++i) {
    auto &i_item = tag[i];
    reader.read_uint32(ptr, i_item);
    ptr += 4;
  }
  int32_t widgets_vec_size;
  reader.read_int32(ptr, widgets_vec_size);
  ptr += 4;
  std::vector<std::unique_ptr<Widget>> widgets(widgets_vec_size);
  for (int i = 0; i < widgets_vec_size; ++i) {
    auto &i_item = widgets[i];
    size_t i_item_bytes_read;
    reader.buffer += ptr;
    i_item = std::move(make_widget(reader, i_item_bytes_read));
    reader.buffer = buf;
    ptr += i_item_bytes_read;
  }
  std::unique_ptr<NanoPack::Message> args = nullptr;
  if (reader.buffer[ptr++] != 0) {
    reader.buffer += ptr;
    size_t args_bytes_read;
    args = std::move(make_nanopack_message(reader, args_bytes_read));
    reader.buffer = buf;
    ptr += args_bytes_read;
  }

  update_widgets(tag, widgets, std::move(args));

  NanoPack::Writer writer(6);
  writer.append_uint8(NanoPack::RpcMessageType::Response);
  writer.append_uint32(msg_id);
  writer.append_uint8(0);

  return {writer.into_data(), writer.size()};
}

NanoPack::RpcServer::MethodCallResult
Rpc::NativeLayerServiceServer::_remove_widget(uint8_t *request_data,
                                              size_t offset,
                                              NanoPack::MessageId msg_id) {
  NanoPack::Reader reader(request_data);
  size_t ptr = offset;
  uint8_t *buf = reader.buffer;
  uint32_t tag;
  reader.read_uint32(ptr, tag);
  ptr += 4;

  remove_widget(tag);

  NanoPack::Writer writer(6);
  writer.append_uint8(NanoPack::RpcMessageType::Response);
  writer.append_uint32(msg_id);
  writer.append_uint8(0);

  return {writer.into_data(), writer.size()};
}

NanoPack::RpcServer::MethodCallResult
Rpc::NativeLayerServiceServer::_insert_widget_before(
    uint8_t *request_data, size_t offset, NanoPack::MessageId msg_id) {
  NanoPack::Reader reader(request_data);
  size_t ptr = offset;
  uint8_t *buf = reader.buffer;
  std::unique_ptr<Widget> widget;
  size_t widget_bytes_read;
  reader.buffer += ptr;
  widget = std::move(make_widget(reader, widget_bytes_read));
  reader.buffer = buf;
  ptr += widget_bytes_read;
  std::unique_ptr<Widget> before_widget;
  size_t before_widget_bytes_read;
  reader.buffer += ptr;
  before_widget = std::move(make_widget(reader, before_widget_bytes_read));
  reader.buffer = buf;
  ptr += before_widget_bytes_read;
  uint32_t parent_tag;
  reader.read_uint32(ptr, parent_tag);
  ptr += 4;

  insert_widget_before(std::move(widget), std::move(before_widget), parent_tag);

  NanoPack::Writer writer(6);
  writer.append_uint8(NanoPack::RpcMessageType::Response);
  writer.append_uint32(msg_id);
  writer.append_uint8(0);

  return {writer.into_data(), writer.size()};
}

NanoPack::RpcServer::MethodCallResult
Rpc::NativeLayerServiceServer::_clear_window(uint8_t *request_data,
                                             size_t offset,
                                             NanoPack::MessageId msg_id) {
  NanoPack::Reader reader(request_data);
  size_t ptr = offset;
  uint8_t *buf = reader.buffer;
  uint32_t window_tag_size;
  reader.read_uint32(ptr, window_tag_size);
  ptr += 4;
  std::string window_tag;
  reader.read_string(ptr, window_tag_size, window_tag);
  ptr += window_tag_size;

  clear_window(window_tag);

  NanoPack::Writer writer(6);
  writer.append_uint8(NanoPack::RpcMessageType::Response);
  writer.append_uint32(msg_id);
  writer.append_uint8(0);

  return {writer.into_data(), writer.size()};
}

std::future<void> Rpc::NativeLayerServiceClient::create_window(
    const std::string &title, const std::string &description, int32_t width,
    int32_t height, const std::string &tag) {
  NanoPack::Writer writer(9 + 13);
  const auto msg_id = new_message_id();
  writer.append_uint8(NanoPack::RpcMessageType::Request);
  writer.append_uint32(msg_id);
  writer.append_uint32(13);
  writer.append_string_view("create_window");
  writer.append_int32(title.size());
  writer.append_string(title);
  writer.append_int32(description.size());
  writer.append_string(description);
  writer.append_int32(width);
  writer.append_int32(height);
  writer.append_int32(tag.size());
  writer.append_string(tag);

  return std::async(
      [this](uint32_t msg_id, uint8_t *req_data, size_t req_size) {
        auto res_data =
            send_request_data_async(msg_id, req_data, req_size).get();
        NanoPack::Reader reader(res_data);
        size_t ptr = 0;
        uint8_t err_flag;
        reader.read_uint8(ptr++, err_flag);
        if (err_flag == 1) {
          throw std::runtime_error("RPC on NativeLayer::create_window failed.");
        }
        free(req_data);
      },
      msg_id, writer.into_data(), writer.size());
}

std::future<void>
Rpc::NativeLayerServiceClient::create_widget(std::unique_ptr<Widget> widget,
                                             const std::string &window_tag) {
  NanoPack::Writer writer(9 + 13);
  const auto msg_id = new_message_id();
  writer.append_uint8(NanoPack::RpcMessageType::Request);
  writer.append_uint32(msg_id);
  writer.append_uint32(13);
  writer.append_string_view("create_widget");
  const size_t widget_byte_size = widget->write_to(writer, writer.size());
  writer.append_int32(window_tag.size());
  writer.append_string(window_tag);

  return std::async(
      [this](uint32_t msg_id, uint8_t *req_data, size_t req_size) {
        auto res_data =
            send_request_data_async(msg_id, req_data, req_size).get();
        NanoPack::Reader reader(res_data);
        size_t ptr = 0;
        uint8_t err_flag;
        reader.read_uint8(ptr++, err_flag);
        if (err_flag == 1) {
          throw std::runtime_error("RPC on NativeLayer::create_widget failed.");
        }
        free(req_data);
      },
      msg_id, writer.into_data(), writer.size());
}

std::future<void>
Rpc::NativeLayerServiceClient::append_new_widget(std::unique_ptr<Widget> child,
                                                 uint32_t parent_tag) {
  NanoPack::Writer writer(9 + 17);
  const auto msg_id = new_message_id();
  writer.append_uint8(NanoPack::RpcMessageType::Request);
  writer.append_uint32(msg_id);
  writer.append_uint32(17);
  writer.append_string_view("append_new_widget");
  const size_t child_byte_size = child->write_to(writer, writer.size());
  writer.append_uint32(parent_tag);

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
              "RPC on NativeLayer::append_new_widget failed.");
        }
        free(req_data);
      },
      msg_id, writer.into_data(), writer.size());
}

std::future<void> Rpc::NativeLayerServiceClient::update_widget(
    uint32_t tag, std::unique_ptr<Widget> widget,
    std::unique_ptr<NanoPack::Message> args) {
  NanoPack::Writer writer(9 + 13);
  const auto msg_id = new_message_id();
  writer.append_uint8(NanoPack::RpcMessageType::Request);
  writer.append_uint32(msg_id);
  writer.append_uint32(13);
  writer.append_string_view("update_widget");
  writer.append_uint32(tag);
  const size_t widget_byte_size = widget->write_to(writer, writer.size());
  if (args != nullptr) {
    writer.append_uint8(1);
    const auto args_value = std::move(args);
    const size_t args_value_byte_size =
        args_value->write_to(writer, writer.size());
  } else {
    writer.append_uint8(0);
  }

  return std::async(
      [this](uint32_t msg_id, uint8_t *req_data, size_t req_size) {
        auto res_data =
            send_request_data_async(msg_id, req_data, req_size).get();
        NanoPack::Reader reader(res_data);
        size_t ptr = 0;
        uint8_t err_flag;
        reader.read_uint8(ptr++, err_flag);
        if (err_flag == 1) {
          throw std::runtime_error("RPC on NativeLayer::update_widget failed.");
        }
        free(req_data);
      },
      msg_id, writer.into_data(), writer.size());
}

std::future<void> Rpc::NativeLayerServiceClient::update_widgets(
    const std::vector<uint32_t> &tag,
    const std::vector<std::unique_ptr<Widget>> &widgets,
    std::unique_ptr<NanoPack::Message> args) {
  NanoPack::Writer writer(9 + 14);
  const auto msg_id = new_message_id();
  writer.append_uint8(NanoPack::RpcMessageType::Request);
  writer.append_uint32(msg_id);
  writer.append_uint32(14);
  writer.append_string_view("update_widgets");
  const size_t tag_vec_size = tag.size();
  writer.append_int32(tag_vec_size);
  for (auto &i : tag) {
    writer.append_uint32(i);
  }
  const size_t widgets_vec_size = widgets.size();
  writer.append_int32(widgets_vec_size);
  int32_t widgets_byte_size = sizeof(int32_t);
  for (auto &j : widgets) {
    const size_t j_byte_size = j->write_to(writer, writer.size());
    widgets_byte_size += j_byte_size;
  }
  if (args != nullptr) {
    writer.append_uint8(1);
    const auto args_value = std::move(args);
    const size_t args_value_byte_size =
        args_value->write_to(writer, writer.size());
  } else {
    writer.append_uint8(0);
  }

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
              "RPC on NativeLayer::update_widgets failed.");
        }
        free(req_data);
      },
      msg_id, writer.into_data(), writer.size());
}

std::future<void> Rpc::NativeLayerServiceClient::remove_widget(uint32_t tag) {
  NanoPack::Writer writer(9 + 13 + 4);
  const auto msg_id = new_message_id();
  writer.append_uint8(NanoPack::RpcMessageType::Request);
  writer.append_uint32(msg_id);
  writer.append_uint32(13);
  writer.append_string_view("remove_widget");
  writer.append_uint32(tag);

  return std::async(
      [this](uint32_t msg_id, uint8_t *req_data, size_t req_size) {
        auto res_data =
            send_request_data_async(msg_id, req_data, req_size).get();
        NanoPack::Reader reader(res_data);
        size_t ptr = 0;
        uint8_t err_flag;
        reader.read_uint8(ptr++, err_flag);
        if (err_flag == 1) {
          throw std::runtime_error("RPC on NativeLayer::remove_widget failed.");
        }
        free(req_data);
      },
      msg_id, writer.into_data(), writer.size());
}

std::future<void> Rpc::NativeLayerServiceClient::insert_widget_before(
    std::unique_ptr<Widget> widget, std::unique_ptr<Widget> before_widget,
    uint32_t parent_tag) {
  NanoPack::Writer writer(9 + 20);
  const auto msg_id = new_message_id();
  writer.append_uint8(NanoPack::RpcMessageType::Request);
  writer.append_uint32(msg_id);
  writer.append_uint32(20);
  writer.append_string_view("insert_widget_before");
  const size_t widget_byte_size = widget->write_to(writer, writer.size());
  const size_t before_widget_byte_size =
      before_widget->write_to(writer, writer.size());
  writer.append_uint32(parent_tag);

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
              "RPC on NativeLayer::insert_widget_before failed.");
        }
        free(req_data);
      },
      msg_id, writer.into_data(), writer.size());
}

std::future<void>
Rpc::NativeLayerServiceClient::clear_window(const std::string &window_tag) {
  NanoPack::Writer writer(9 + 12);
  const auto msg_id = new_message_id();
  writer.append_uint8(NanoPack::RpcMessageType::Request);
  writer.append_uint32(msg_id);
  writer.append_uint32(12);
  writer.append_string_view("clear_window");
  writer.append_int32(window_tag.size());
  writer.append_string(window_tag);

  return std::async(
      [this](uint32_t msg_id, uint8_t *req_data, size_t req_size) {
        auto res_data =
            send_request_data_async(msg_id, req_data, req_size).get();
        NanoPack::Reader reader(res_data);
        size_t ptr = 0;
        uint8_t err_flag;
        reader.read_uint8(ptr++, err_flag);
        if (err_flag == 1) {
          throw std::runtime_error("RPC on NativeLayer::clear_window failed.");
        }
        free(req_data);
      },
      msg_id, writer.into_data(), writer.size());
}
