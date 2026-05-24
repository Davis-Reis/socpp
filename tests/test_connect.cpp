#include "socpp/socket.hpp"

#include <cassert>
#include <thread>
#include <chrono>

int main() {
    constexpr std::uint16_t port = 19000;

    // open a thread and have it listen to a port.
    std::thread server([] {
            auto listener = socpp::Socket::listen(port);
            auto client = listener.accept(); // Blocks! waits for message

            char buf[6] = {};
            client.read_exact(buf, 5);
            assert(strcmp(buf, "hello") == 0);

            client.write_all(buf, 5);
            });
    
    // sleep this thread so listen has a chance to run
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    auto sock = socpp::Socket::connect("127.0.0.1", port);
    sock.write_all("hello", 5); // Server should wake up now that there is something to listen

    // Client now wrote, check if its inside
    char buf[6] = {};
    sock.read_exact(buf, 5);
    assert(strcmp(buf, "hello") == 0);

    server.join();

    return 0;
}
