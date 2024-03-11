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
#include <unordered_set>
#include <vector>

namespace Poly {

typedef uint32_t RequestId;

typedef std::function<void(std::unique_ptr<NanoPack::Message> message)>
	MessageHandler;
typedef std::function<void(int error)> ErrorHandler;
typedef std::function<void(NanoPack::Any retval)> CallbackReturn;

/// @brief Provides an interface to interact with the portable layer, such as
/// sending/receiving messages.
class PortableLayer {
	int pid;
	int stdin_handle;
	int stdout_handle;
	std::filesystem::path bin_path;
	std::unordered_set<RequestId> pending_request_ids;
	std::unordered_map<int32_t, std::promise<NanoPack::Any>> pending_callback;
	Random<int32_t> random_reply_handle;
	Random<RequestId> random_request_id;

	MessageHandler message_handler;
	ErrorHandler error_handler;

	void read_portable_layer_stdout();

	void reply_to_callback(const Message::ReplyFromCallback *msg);

	void handle_message(RequestId request_id, std::vector<uint8_t> msg_bytes);

	void handle_request_ack(RequestId request_id);

	void send_request_ack(RequestId request_id) const;

	RequestId generate_request_id();

  public:
	explicit PortableLayer(std::filesystem::path bin_path);

	void send_message(const NanoPack::Message &message);

	void invoke_callback(int32_t callback_handle, NanoPack::Any args);

	std::future<NanoPack::Any>
	invoke_callback_with_result(int32_t callback_handle, NanoPack::Any args);

	void on_message(const MessageHandler &handler);

	void on_error(const ErrorHandler &handler);

	/// @brief Spawns the portable layer in a child process. This must be called
	/// before you can do anything else with the portable layer.
	void spawn();

	void terminate();
};
} // namespace Poly

#endif // GTKPOLY_PORTABLE_LAYER_HXX
