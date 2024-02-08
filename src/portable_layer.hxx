//
// Created by kenym on 07/02/24.
//

#ifndef GTKPOLY_PORTABLE_LAYER_HXX
#define GTKPOLY_PORTABLE_LAYER_HXX

#include "nanopack/any.hxx"

#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <vector>

namespace Poly {

typedef std::function<void(std::unique_ptr<NanoPack::Message> message)> MessageHandler;;
typedef std::function<void(int error)> ErrorHandler;

class PortableLayer {
  private:
	int pid;
	int stdin_handle;
	int stdout_handle;
	std::filesystem::path bin_path;

	MessageHandler message_handler;
	ErrorHandler error_handler;

	void read_portable_layer_stdout();

	void handle_message(std::vector<uint8_t> msg_bytes);

  public:
	explicit PortableLayer(std::filesystem::path bin_path);

	void on_message(const MessageHandler& handler);

	void on_error(ErrorHandler handler);

	/**
	 * \brief Spawns the portable layer in a child process.
	 */
	void spawn();

	void terminate();
};

} // namespace Poly

#endif // GTKPOLY_PORTABLE_LAYER_HXX
