#include "socpp/socket.hpp"

#include <unistd.h>

namespace socpp {

// Constructors and destructors
Socket::Socket() {
    ::socket(_fd);
}

Socket::Socket(int fd) : fd_(fd) {
    ::socket(_fd);
}

Socket::~Socket() {
    close();
}

Socket::Socket(Socket&& other) noexcept : fd_(other.fd_) {
    other.fd_ = -1;
}

Socket& Socket::operator=(Socket&& other) noexcept {
    if (this != &other) {
        close(fd_);
        fd_ = other.fd_;
        other.fd_ = -1;
    }
    return *this;
}

bool Socket::valid() const noexcept {
    return fd_ != -1;
}

int fd() const noexcept {
    return fd_;
}

void close() noexcept {
    if (valid()) {
        ::close(fd_);
        fd_ = -1;
    }
}

} // namespace socpp
