#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <errno.h>

#include "common/common.h"
#include "io/blocking.h"

static int32_t one_request(int connfd)
{
    // Parse Request
    // Request structure:
    // (len) 4 bytes + (msg) len bytes + NULL

    // Parse header
    char request_buf[4 + k_max_msg + 1];
    errno = 0;
    int32_t err = read_full(connfd, request_buf, 4);
    if (err) {
        if (errno == 0) {
            msg("EOF");
        } else {
            msg("read() header error");
        }
        return err;
    }

    uint32_t len = 0;
    memcpy(&len, request_buf, 4);
    if (len > k_max_msg) {
        msg("too long");
        return -1;
    }

    // parse message
    err = read_full(connfd, &request_buf[4], len);
    if (err) {
        msg("read() message error");
        return err;
    }

    // Process
    request_buf[4 + len] = '\0';
    printf("Client: %s\n", &request_buf[4]);

    // Reply
    const char reply[] = "world";
    char wbuf[4 + sizeof(reply)];
    len = (uint32_t) strlen(reply);
    memcpy(wbuf, &len, 4);
    memcpy(&wbuf[4], reply, len);
    return write_all(connfd, wbuf, 4 + len);
}

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        die("socket()");
    }

    // set socket options
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)); // set SO_REUSEADDR to 1 at socket level

    // bind
    struct sockaddr_in addr = {}; // header: netinet/in.h
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(0); // for 0.0.0.0, IP address is 32 bytes (short)

    int rv = bind(fd, (const sockaddr *)&addr, sizeof(addr));
    if (rv)
    {
        die("bind()");
    }

    rv = listen(fd, SOMAXCONN);
    if (rv)
    {
        die("listen()");
    }

    while (true)
    {
        struct sockaddr_in client_addr = {};
        socklen_t socklen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
        if (connfd < 0)
        {
            continue;
        }
        while (true)
        {
            int32_t err = one_request(connfd);
            if (err)
            {
                break;
            }
        }
        close(connfd);
    }

    return 0;
}
