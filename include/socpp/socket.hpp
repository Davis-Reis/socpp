#pragma once

namespace socpp {

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
    // Some magic needs to happen here to trim off the TCP header.
    void read_exact(void* buf, std::size_t size);

    // Writes bytes from buf to socket datastream
    void write_all(const void* buf, std::size_t size);
    

private:
    int fd_ = -1;
};

} // namespace socpp
