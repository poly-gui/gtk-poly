//
// Created by kenym on 07/02/24.
//

#include <csignal>
#include <unistd.h>

#include "messages/nanopack_message_factory.np.hxx"
#include "portable_layer.hxx"

#include <iostream>
#include <thread>

Poly::PortableLayer::PortableLayer(std::filesystem::path bin_path)
	: pid(-1), stdin_handle(-1), stdout_handle(-1),
	  bin_path(std::move(bin_path)), message_handler(nullptr),
	  error_handler(nullptr) {
}

void Poly::PortableLayer::on_message(const MessageHandler &handler) {
	message_handler = handler;
}

void Poly::PortableLayer::on_error(const ErrorHandler handler) {
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
		std::cout << "fork" << std::endl;
		std::cout << bin_path << std::endl;

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
			handle_message(std::move(msg_bytes));
		} else if (status == 0) {
			// unexpected EOF, pipe closed unexpectedly
			// TODO: handle this
		} else if (error_handler != nullptr) {
			error_handler(errno);
		}
	}
}

void Poly::PortableLayer::handle_message(std::vector<uint8_t> msg_bytes) {
	int bytes_read;
	std::unique_ptr<NanoPack::Message> message =
		Message::make_nanopack_message(msg_bytes.begin(), bytes_read);
	if (message_handler != nullptr) {
		message_handler(std::move(message));
	}
}
