#pragma once

#include <stdexcept>
#include <cstdint>
#include <cstring>
#include <cerrno>
#include <system_error>

namespace socpp {

class SocketError : public std::runtime_error {
public:
    explicit SocketError(const std::string& msg) : std::runtime_error(msg) {}
};

class ConnectionError : public std::runtime_error {
public:
    explicit ConnectionError(const std::string& msg) : std::runtime_error(msg) {}
};

class Socket {
public:
    // Constuctors and destructors. Allows for generic socket creation, or specific
    Socket() noexcept = default;
    explicit Socket(int fd) noexcept;
    ~Socket();
    
    // Copy prevention, two sockets should not be able to hold the same fd
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    // Move operations, sockets can transfer ownership but must clone previous socket
    // move a -> b: b = a, close(a)
    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;

    // For testing, checks if this socket is holding a valid fd
    bool valid() const noexcept;

    // Returns the fd of this socket
    int fd() const noexcept;

    // Close current socket
    void close() noexcept;

    // Reads size amount of bytes and stores them in buf
    void read_exact(void* buf, std::size_t size);

    // Writes bytes from buf to socket datastream
    void write_all(const void* buf, std::size_t size);

    // !!! Factories !!!
    // Creates a socket for a specific purpose

    // Connect socket 
    // Call ::socket(AF_INET, SOCK_STREAM, ...)
    // Wrap it in a Socket
    // Build a sockaddr_in with IP and port (inet_pton(AF_INET, "127.0.0.1", &(sockaddr_in)))
    // Call ::connect(sock.fd(), ...) Throw error on fail
    // Return socket
    static Socket connect(const std::string& ip, std::uint16_t port);

    // Listen socket
    // Call ::socket(AF_INET, SOCK_STREAM, 0).
    // Wrap it in a Socket.
    // Build a sockaddr_in with INADDR_ANY and the port.
    // Call ::bind(fd, ...). Throw on failure.
    // Call ::listen(fd, backlog). Throw on failure.
    // Return the Socket.
    static Socket listen(std::uint16_t port);
    
    // Accept socket
    // Call ::accept(fd_, ...) on the listening socket.
    // Throw on failure.
    // Return a new Socket wrapping the returned fd.
    Socket accept();
    

private:
    int fd_ = -1;
};

} // namespace socpp
