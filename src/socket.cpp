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

void Socket::read_exact(void* buf, std::size_t size) {
    // void* arithmetic causes a compile error so cast to uint_8
    auto* p = static_cast<std::uint8_t>(buf);
    std::size_t bytes_read = 0;
    while(bytes_read < size) {
        // pointer arithmetic here is used to append message onto the end of the buffer.
        //         (fd ,   memory addr   , remaining length )
        n = ::recv(fd_, buf + bytes_read, bytes_read - size);
        
        // Success
        if(n > 0) {
            bytes_read += n;
        }

        // Closed connection
        else if(n == 0) {
            throw ConnectionError("Connection closed by peer");
        }

        // Error
        else {
            // Error from systemcallinterupt
            if (errno == EINTR) continue;
            throw SocketError("recv failed: errno = " + std::string(std::errno(errno)));
        }

}

} // namespace socpp
