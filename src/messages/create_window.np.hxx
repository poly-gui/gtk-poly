// AUTOMATICALLY GENERATED BY NANOPACK. DO NOT MODIFY BY HAND.

#ifndef CREATE_WINDOW_NP_HXX
#define CREATE_WINDOW_NP_HXX

#include <nanopack/message.hxx>
#include <nanopack/nanopack.hxx>
#include <nanopack/reader.hxx>
#include <string>
#include <vector>

namespace Poly::Message {

struct CreateWindow : NanoPack::Message {
  static constexpr NanoPack::TypeId TYPE_ID = 3533765426;

  std::string title;
  std::string description;
  int32_t width;
  int32_t height;
  std::string tag;

  CreateWindow() = default;

  CreateWindow(std::string title, std::string description, int32_t width,
               int32_t height, std::string tag);

  CreateWindow(std::vector<uint8_t>::const_iterator begin, int &bytes_read);

  CreateWindow(const NanoPack::Reader &reader, int &bytes_read);

  size_t write_to(std::vector<uint8_t> &buf, int offset) const override;

  [[nodiscard]] NanoPack::TypeId type_id() const override;

  [[nodiscard]] int header_size() const override;

  [[nodiscard]] std::vector<uint8_t> data() const override;
};

} // namespace Poly::Message

#endif
