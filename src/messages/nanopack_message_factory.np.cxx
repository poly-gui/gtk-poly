// AUTOMATICALLY GENERATED BY NANOPACK. DO NOT MODIFY BY HAND.

#include "nanopack_message_factory.np.hxx"

#include "create_window.np.hxx"
#include "invoke_callback.np.hxx"
#include "widgets/button/button.np.hxx"
#include "widgets/button/click_event.np.hxx"
#include "widgets/center.np.hxx"
#include "widgets/column.np.hxx"
#include "widgets/create_widget.np.hxx"
#include "widgets/text.np.hxx"
#include "widgets/update_widget.np.hxx"
#include "widgets/update_widgets.np.hxx"
#include "widgets/widget.np.hxx"

std::unique_ptr<NanoPack::Message> Poly::Message::make_nanopack_message(
    std::vector<uint8_t>::const_iterator data_iter, int &bytes_read) {
  switch (const NanoPack::Reader reader(data_iter); reader.read_type_id()) {
  case 1041:
    return std::make_unique<ClickEvent>(reader, bytes_read);
  case 100:
    return std::make_unique<Widget>(reader, bytes_read);
  case 102:
    return std::make_unique<Center>(reader, bytes_read);
  case 10:
    return std::make_unique<CreateWindow>(reader, bytes_read);
  case 2:
    return std::make_unique<InvokeCallback>(reader, bytes_read);
  case 103:
    return std::make_unique<Column>(reader, bytes_read);
  case 20:
    return std::make_unique<CreateWidget>(reader, bytes_read);
  case 4:
    return std::make_unique<UpdateWidgets>(reader, bytes_read);
  case 101:
    return std::make_unique<Text>(reader, bytes_read);
  case 104:
    return std::make_unique<Button>(reader, bytes_read);
  case 3:
    return std::make_unique<UpdateWidget>(reader, bytes_read);
  default:
    return nullptr;
  }
}
