#include "socpp/socket.hpp"

int main() {
    int sv[2];
    ::socketpair(sv);
    socpp::Socket writer(sv[0]);
    socpp::Socket reader(sv[1]);

    const char* msg = "hello";
    ::write(writer, msg, 5);

    char buf[6] = {};
    reader.read_exact(buf, 5);
    assert(std::strcmp(buf, "hello") == 0);

    ::close(sv[1]);
    return 0;
}
