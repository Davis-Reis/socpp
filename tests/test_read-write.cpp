#include "socpp/socket.hpp"

#include <cassert>
#include <fcntl.h>
#include <utility>
#include <sys/socket.h>

int main() {
    // { // Preliminary socket test
    //     int sv[2];
    //     ::socketpair(sv);
    //     socpp::Socket writer(sv[0]);
    //     socpp::Socket reader(sv[1]);
    //
    //     const char* msg = "hello";
    //     ::write(writer, msg, 5);
    //
    //     char buf[6] = {};
    //     reader.read_exact(buf, 5);
    //     assert(std::strcmp(buf, "hello") == 0);
    //
    //     ::close(sv[1]);
    // }

    // TODO: make more verbose tests!
    return 0;
}
