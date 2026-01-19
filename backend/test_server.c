#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "HTTP-router.h"
#include "Server.h"
#include "db.h"

void launch(server_t *server)
{
    char buffer[30000];
    int address_length = sizeof(server->address);

    while (1)
    {
        int client = accept(server->socket,
                            (struct sockaddr *)&server->address, (socklen_t *)&address_length);
        int bytes = read(client, buffer, sizeof(buffer) - 1);

        if (bytes <= 0)
        {
            close(client);
            continue;
        }
        buffer[bytes] = '\0';

        handle_request(client, buffer);

        close(client);
    }
};

int main()
{
    if ((global_conn = db_connect()) == NULL)
    {
        perror("Error connecting to db");
    }

    server_t server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 3000, 10, launch);
    server.launch(&server);

    PQfinish(global_conn);
    return 0;
}