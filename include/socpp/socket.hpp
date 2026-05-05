#pragma once

namespace socpp {

class Socket {
public:
    Socket() noexcept = default;
    explicit Socket(int fd) noexcept;
    ~Socket();
    
    // Copy prevention
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    // Move operations
    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;

    bool valid() const noexcept;

    int fd() const noexcept;

    void close() noexcept;

    

private:
    int fd_ = -1;
};

} // namespace socpp
