#include "socpp/socket.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

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
    auto* p = static_cast<std::uint8_t*>(buf);
    std::size_t bytes_read = 0;
    while(bytes_read < size) {
        // pointer arithmetic here is used to append message onto the end of the buffer.
        //                (fd ,   memory addr ,  remaining length   )
        ssize_t n = ::recv(fd_, p + bytes_read, size - bytes_read, 0);
        
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
            throw SocketError("recv failed: errno = " + std::string(std::strerror(errno)));
        }
    }
}

void Socket::write_all(const void* buf, std::size_t size) {
    auto* p = static_cast<const std::uint8_t*>(buf);
    std::size_t bytes_sent = 0;

    while(bytes_sent < size) {
        // pointer arithmaetic is same as read_exact
        ssize_t n = ::send(fd_, p + bytes_sent, size - bytes_sent, 0);

        // Success
        if (n >= 0) {
            bytes_sent += n;
        }
        // Error
        else {
            if (errno == EINTR) continue;
            throw SocketError("send failed: errno = " + std::string(std::strerror(errno)));
        }
    }
}

Socket Socket::connect(const std::string& ip, std::uint16_t port) {
    int s, con;
    Socket socket;
    struct sockaddr_in addr;


    s = ::socket(AF_INET, SOCK_STREAM, 0);
    socket = Socket(s);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    
    ::inet_pton(AF_INET, ip.c_str(), &(addr.sin_addr));

    int fd = socket.fd();
    con = ::connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));

    if (con != 0) {
        throw SocketError("connection failed: errno = " + std::string(std::strerror(errno)));
    }
    else {
        return socket;
    }
}

Socket Socket::listen(std::uint16_t port) {
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        throw SocketError("socket failed: " + std::string(std::strerror(errno)));
    }
    Socket sock(fd);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (::bind(sock.fd(), reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
        throw SocketError("bind() failed: " + std::string(std::strerror(errno)));
    }

    if (::listen(sock.fd(), SOMAXCONN) == -1) {
        throw SocketError("listen() failed: " + std::string(std::strerror(errno))); }
    return sock;
}

Socket Socket::accept() {
    int client_fd = ::accept(fd_ , nullptr, nullptr);

    if (client_fd == -1) {
        throw SocketError("accept failed: errno = " + std::string(std::strerror(errno)));
    }
    else {
        return Socket(client_fd);
    }
}

} // namespace socpp
