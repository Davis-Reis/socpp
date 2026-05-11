#include "socpp/socket.hpp"

#include <cassert>
#include <fcntl.h>
#include <unistd.h>
#include <utility>

/**
 * Function to check if a socket is still taking up fd int
 */
bool fd_is_open(int fd) {
    return ::fcntl(fd, F_GETFD) != -1;
}

int main() {
    // Test: if Socket falling out of scope calls destructor properly
    {
        // Get a real fd we can wrap
        int pipefd[2];
        assert(::pipe(pipefd) == 0);
        int read_end = pipefd[0];
        int write_end = pipefd[1];

        // Sanity check: fd is open right now
        assert(fd_is_open(read_end));

        {
            socpp::Socket s(read_end);
            // Sanity checks: s is valid, s is set to correct fd, fd is open
            assert(s.valid());
            assert(s.fd() == read_end);
            assert(fd_is_open(read_end));
        }   // Run destructor

        // Check to make sure fd is now gone
        assert(!fd_is_open(read_end));

        ::close(write_end);
    }

    {
        socpp::Socket s;
        assert(!s.valid());
        assert(s.fd() == -1);
    }

    {
        int pipefd[2];
        ::pipe(pipefd);
        {
            socpp::Socket a(pipefd[0]);
            socpp::Socket b(std::move(a));

            assert(!a.valid());
            assert(b.valid());
            assert(b.fd() == pipefd[0]);
            assert(a.fd() == -1);
            assert(fd_is_open(pipefd[0]));
        }   // Rune destructor

        assert(!fd_is_open(pipefd[0]));

        ::close(pipefd[1]);
    }
    
    {
        int p1[2], p2[2];
        ::pipe(p1);
        ::pipe(p2);
        {
            socpp::Socket a(p1[0]);
            socpp::Socket b(p2[0]);

            b = std::move(a);

            assert(!a.valid());
            assert(b.valid());
            assert(b.fd() == p1[0]);
            assert(fd_is_open(p1[0]));
            assert(!fd_is_open(p2[0]));
        }
        assert(!fd_is_open(p1[0]));

        ::close(p1[1]);
        ::close(p2[1]);
    }
    return 0;
}
