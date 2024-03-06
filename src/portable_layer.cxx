//
// Created by kenym on 07/02/24.
//

#include <csignal>
#include <unistd.h>

#include "messages/nanopack_message_factory.np.hxx"
#include "portable_layer.hxx"

#include "messages/invoke_callback.np.hxx"
#include "messages/reply_from_callback.np.hxx"
#include "messages/request.np.hxx"
#include "messages/ok_response.np.hxx"

#include <iostream>
#include <thread>
#include <utility>

Poly::PortableLayer::PortableLayer(std::filesystem::path bin_path)
	: pid(-1), stdin_handle(-1), stdout_handle(-1),
	  bin_path(std::move(bin_path)),
	  random_reply_handle(0, std::numeric_limits<int32_t>::max()),
	  message_handler(nullptr), error_handler(nullptr) {}

void Poly::PortableLayer::send_message(const NanoPack::Message &message) const {
	std::vector<uint8_t> msg_bytes = message.data_with_length_prefix();
	write(stdin_handle, msg_bytes.data(), msg_bytes.size());
}

void Poly::PortableLayer::invoke_callback(const int32_t callback_handle,
										  NanoPack::Any args) const {
	const Message::InvokeCallback invoke_callback(
		callback_handle, std::move(args), std::nullopt);
	send_message(invoke_callback);
}

std::future<NanoPack::Any>
Poly::PortableLayer::invoke_callback_with_result(const int32_t callback_handle,
												 NanoPack::Any args) {
	std::promise<NanoPack::Any> promise;
	auto future = promise.get_future();

	int32_t reply_handle = random_reply_handle.generate();
	pending_callback[reply_handle] = std::move(promise);

	const Message::InvokeCallback invoke_callback(
		callback_handle, std::move(args), reply_handle);
	send_message(invoke_callback);

	return future;
}

void Poly::PortableLayer::on_message(const MessageHandler &handler) {
	message_handler = handler;
}

void Poly::PortableLayer::on_error(const ErrorHandler &handler) {
	error_handler = handler;
}

void Poly::PortableLayer::spawn() {
	constexpr int READ_FD = 0;
	constexpr int WRITE_FD = 1;

	// receive message from portable layer through this pipe
	int child_stdout_pipe[2];
	// send message to portable layer through this pipe
	int child_stdin_pipe[2];

	pipe(child_stdin_pipe);
	pipe(child_stdout_pipe);

	const int pid = fork();
	if (const bool is_child = pid == 0; is_child) {
		// file descriptor of stdout of native layer
		// messages from native layer are sent to here which can be read with
		// the read descriptor (READ_FD)
		const int parent_stdout = child_stdin_pipe[READ_FD];
		// file descriptor of stdin of parent process
		// portable layer will send messages through here by *writing* from here
		// with the write descriptor (WRITE_FD)
		const int parent_stdin = child_stdout_pipe[WRITE_FD];

		//                   (1)
		//          stdout ------> stdin
		//        /                      \
		//  native                        portable
		//        \                      /
		//          stdin  <------ stdout
		//                   (2)

		// pipe stdout of native layer to stdin of poratble layer
		// (1)
		dup2(parent_stdout, STDIN_FILENO);
		// pipe stdout of portable layer to stdin of native layer
		// (2)
		dup2(parent_stdin, STDOUT_FILENO);

		close(child_stdin_pipe[WRITE_FD]);
		close(child_stdout_pipe[READ_FD]);

		const char *bin_path_c_str = bin_path.c_str();
		execl(bin_path_c_str, bin_path_c_str, nullptr);

		std::cout << "should not be here: " << errno << std::endl;

		// TODO: handle when portable layer exits early here.
	} else {
		this->pid = pid;
		stdin_handle = child_stdin_pipe[WRITE_FD];
		stdout_handle = child_stdout_pipe[READ_FD];

		close(child_stdin_pipe[READ_FD]);
		close(child_stdout_pipe[WRITE_FD]);

		std::thread t(&PortableLayer::read_portable_layer_stdout, this);
		t.detach();
	}
}

void Poly::PortableLayer::terminate() {
	if (pid > 0) {
		close(stdin_handle);
		close(stdout_handle);
		kill(pid, SIGKILL);
		pid = -1;
	}
}

void Poly::PortableLayer::read_portable_layer_stdout() {
	uint8_t msg_size_buf[sizeof(uint32_t)];
	while (read(stdout_handle, msg_size_buf, sizeof(uint32_t))) {
		uint32_t msg_size = 0;
		msg_size |= msg_size_buf[0];
		msg_size |= msg_size_buf[1] << 8;
		msg_size |= msg_size_buf[2] << 16;
		msg_size |= msg_size_buf[3] << 24;
		std::vector<uint8_t> msg_bytes(msg_size);
		if (const long status = read(stdout_handle, msg_bytes.data(), msg_size);
			status > 0) {
			// for each message received from the portable layer
			// spawn a new thread to handle the message.
			std::thread t(&PortableLayer::handle_message, this,
						  std::move(msg_bytes));
			t.detach();
		} else if (status == 0) {
			// unexpected EOF, pipe closed unexpectedly
			// TODO: handle this
		} else if (error_handler != nullptr) {
			error_handler(errno);
		}
	}
}

void Poly::PortableLayer::reply_to_callback(
	const Message::ReplyFromCallback *msg) {
	const auto entry = pending_callback.find(msg->to);
	if (entry == pending_callback.end())
		return;

	auto promise = std::move(entry->second);
	promise.set_value(std::move(msg->args));

	pending_callback.erase(msg->to);
}

void Poly::PortableLayer::handle_message(std::vector<uint8_t> msg_bytes) {
	int bytes_read;
	std::unique_ptr<NanoPack::Message> message =
		Message::make_nanopack_message(msg_bytes.begin(), bytes_read);
	if (message == nullptr) {
		const std::string verbose(msg_bytes.begin(), msg_bytes.end());
		std::cout << "VERBOSE: " << verbose << std::endl;
		return;
	}

	switch (message->type_id()) {
	case Message::Request::TYPE_ID:
		handle_request(std::move(message));
		break;

	default:
		break;
	}
}

void Poly::PortableLayer::handle_request(
	std::unique_ptr<NanoPack::Message> message) {
	const auto request = static_cast<Message::Request *>(message.get());

	std::vector<uint8_t> body_data = std::move(request->body.data());
	int bytes_read;
	std::unique_ptr<NanoPack::Message> message_body = Message::make_nanopack_message(body_data.begin(), bytes_read);

	if (message_body->type_id() == Message::ReplyFromCallback::TYPE_ID) {
		reply_to_callback(
			static_cast<Message::ReplyFromCallback *>(message_body.get()));
	} else if (message_handler != nullptr) {
		message_handler(std::move(message_body));
		const Message::OkResponse ok_response(request->id, std::nullopt);
		send_message(ok_response);
	}
}
