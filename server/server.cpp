#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <errno.h>

static void msg(const char *msg) {
    fprintf(stderr, "%s\n", msg);
}

static void do_something(int connfd) {
    char rbuf[64] = {};
    ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        msg("read() error");
        return;
    }
    printf("client: %s\n", rbuf);

    char wbuf[] = "world";
    write(connfd, wbuf, strlen(wbuf));
}

static void die(const char *msg) {
    int err = errno;    // header: errno
    fprintf(stderr, "[%d] %s\n", err, msg); // header: stdio
    abort();
}

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        die("socket()");
    }

    // set socket options
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)); // set SO_REUSEADDR to 1 at socket level 

    // bind
    struct sockaddr_in addr = {};   // header: netinet/in.h
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(0); // for 0.0.0.0, IP address is 32 bytes (short)

    int rv = bind(fd, (const sockaddr *) &addr, sizeof(addr));
    if (rv) {
        die("bind()");
    }

    rv = listen(fd, SOMAXCONN);
    if (rv) {
        die("listen()");
    }
    
    while (true)
    {
        struct sockaddr_in client_addr = {};
        socklen_t socklen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr*) &client_addr, &socklen);
        if (connfd < 0) {
            continue;
        }
        do_something(connfd);
        close(connfd);
    }

    return 0;
}
