#include "socpp/socket.hpp"

#include <unistd.h>

namespace socpp {

// Constructors and destructors

Socket::Socket(int fd) noexcept : fd_(fd) {}

Socket::~Socket() {
    close();
}

Socket::Socket(Socket&& other) noexcept : fd_(other.fd_) {
    other.fd_ = -1;
}

Socket& Socket::operator=(Socket&& other) noexcept {
    if (this != &other) {
        close();
        fd_ = other.fd_;
        other.fd_ = -1;
    }
    return *this;
}

bool Socket::valid() const noexcept {
    return fd_ != -1;
}

int Socket::fd() const noexcept {
    return fd_;
}

void Socket::close() noexcept {
    if (valid()) {
        ::close(fd_);
        fd_ = -1;
    }
}

void read_exact(void* buf, std::size_t size) {
    
}

} // namespace socpp
