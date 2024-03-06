//
// Created by kenym on 07/02/24.
//

#ifndef GTKPOLY_PORTABLE_LAYER_HXX
#define GTKPOLY_PORTABLE_LAYER_HXX

#include "messages/reply_from_callback.np.hxx"
#include "random.hxx"

#include <cstdint>
#include <filesystem>
#include <functional>
#include <future>
#include <memory>
#include <nanopack/any.hxx>
#include <nanopack/message.hxx>
#include <unordered_map>
#include <vector>

namespace Poly {

typedef std::function<void(std::unique_ptr<NanoPack::Message> message)>
	MessageHandler;
typedef std::function<void(int error)> ErrorHandler;
typedef std::function<void(NanoPack::Any retval)> CallbackReturn;

/// @brief Provides an interface to interact with the portable layer, such as sending/receiving messages.
class PortableLayer {
	int pid;
	int stdin_handle;
	int stdout_handle;
	std::filesystem::path bin_path;
	std::unordered_map<int32_t, std::promise<NanoPack::Any>> pending_callback;
	Random<int32_t> random_reply_handle;

	MessageHandler message_handler;
	ErrorHandler error_handler;

	void read_portable_layer_stdout();

	void reply_to_callback(const Message::ReplyFromCallback *msg);

	void handle_message(std::vector<uint8_t> msg_bytes);

	void handle_request(std::unique_ptr<NanoPack::Message> message);

  public:
	explicit PortableLayer(std::filesystem::path bin_path);

	void send_message(const NanoPack::Message &message) const;

	void invoke_callback(int32_t callback_handle, NanoPack::Any args) const;

	std::future<NanoPack::Any>
	invoke_callback_with_result(int32_t callback_handle, NanoPack::Any args);

	void on_message(const MessageHandler &handler);

	void on_error(const ErrorHandler &handler);

	/// @brief Spawns the portable layer in a child process. This must be called before you can do anything else with the portable layer.
	void spawn();

	void terminate();
};
} // namespace Poly

#endif // GTKPOLY_PORTABLE_LAYER_HXX
