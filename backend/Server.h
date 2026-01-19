#ifndef Server_h
#define Server_h

#include <sys/socket.h>
#include <netinet/in.h>

typedef struct Server
{
    int domain;
    int service;
    int protocol;
    u_long interface;
    int port;
    int backlog;

    struct sockaddr_in address;

    int socket;

    void (*launch)(struct Server *);
} server_t;

server_t server_constructor(int domain, int service, int protocol,
                            u_long interface, int port, int backlog,
                            void (*launch)(server_t *server));

#endif