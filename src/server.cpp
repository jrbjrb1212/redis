#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>

#include "common.hpp"
#include "redis_store.hpp"
#include "resp_parser.hpp"
#include "resp_handler.hpp"
#include "helpful.hpp"

const size_t PORT = 6379;

static void msg(const char *msg) {
    fprintf(stderr, "%s\n", msg);
}


static void die(const char *msg) {
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}


// workflow: 
// - read from the client file descriptor (socket) until there is no more data
// - The read data will be a RESP formatted request
// - Use the RESPparser to parse it
// - Act on the parsed data
// - Write the result back to the client file descriptor
//      - I'll be able to see this using the redis-cli
// Future TODO: handle arbitrary large requests
void handleClientRequest(int clientFd, RedisStore &dataStore){

    // Read from client
    // - current max request is 256
    // - can handle arbitrary large by viewing the bulk string lengths
    char rbuf[256] = {};
    ssize_t n = read(clientFd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        msg("read() error");
        return;
    }
    RESPParser parser;
    RESPHandler handler(dataStore);
    std::string respMsg = rbuf;
    std::vector<std::string> parsedMsg = parser.deserialize(respMsg);
    
    printVector(rbuf, parsedMsg);

    std::string response = handler.handleMsg(parsedMsg);
    std::cout << "Response: " << response << std::endl;

    write(clientFd, response.c_str(), response.size());
}


int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        die("socket()");
    }

    // this is needed for most server applications
    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    // bind
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(PORT);
    addr.sin_addr.s_addr = ntohl(0);    // wildcard address 0.0.0.0
    int rv = bind(fd, (const sockaddr *)&addr, sizeof(addr));
    if (rv) {
        die("bind()");
    }

    // listen
    rv = listen(fd, SOMAXCONN);
    if (rv) {
        die("listen()");
    }
    printf("Server running on port %d...\n", PORT);

    RedisStore dataStore;

    while (true) {
        // accept
        struct sockaddr_in client_addr = {};
        socklen_t socklen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
        if (connfd < 0) {
            continue;   // error
        }
        handleClientRequest(connfd, dataStore);

        close(connfd);
    }

    return 0;
}