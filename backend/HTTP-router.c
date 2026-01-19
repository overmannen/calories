#include "HTTP-router.h"
#include <stdio.h>
#include <string.h>
#include "db.h"
#include <stdlib.h>

void handle_request(int client, char *request)
{
    char method[8];
    char path[256];

    sscanf(request, "%7s %255s", method, path);

    if (strcmp(method, "POST") == 0)
    {
        char *body = strstr(request, "\r\n\r\n");

        if (!body)
        {
            send_400(client);
            return;
        }

        body += 4;

        if (strcmp(path, "/api/add_athlete") == 0)
        {
            fprintf(stderr, "hei1");
            handle_add_athlete(client, body);
        }
        else
        {
            fprintf(stderr, "hei2");
            int id;
            if (sscanf(path, "/api/athlete/%d", &id) == 1)
            {
                update_athlete(client, id, body);
            }
            else
            {
                send_404(client);
            }
        }
    }
    else if (strcmp(method, "GET") == 0)
    {
        if (strcmp(path, "/api/status") == 0)
        {
            handle_status(client);
        }
        else if (strcmp(path, "/api/athletes") == 0)
        {
            get_athletes(client);
        }
        else
        {
            send_404(client);
        }
    }
    else
    {
        send_404(client);
    }
}

void handle_status(int client)
{
    const char *json = "{ \"status\": \"ok\", \"service\": \"c-rest\" }";

    dprintf(client,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %lu\r\n"
            "\r\n"
            "%s",
            strlen(json),
            json);
}

void send_404(int client)
{
    char *json = "{ \"error\": \"Not found\" }";

    dprintf(client,
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %lu\r\n"
            "\r\n"
            "%s",
            strlen(json),
            json);
}

void send_400(int client)
{
    const char *json = "{ \"error\": \"Bad request\" }";

    dprintf(client,
            "HTTP/1.1 400 Bad Request\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %lu\r\n"
            "\r\n"
            "%s",
            strlen(json),
            json);
}

void handle_add_athlete(int client, char *body)
{

    char name[256];

    sscanf(body, "name=%255s", name);

    char query[512];
    snprintf(query, sizeof(query),
             "INSERT INTO athletes (name, calories) VALUES ('%s', 0)",
             name);
    if (db_execute(global_conn, query) == 0)
    {
        send_400(client);
        return;
    }

    char response[512];
    snprintf(response, sizeof(response),
             "{\"message\": \"%s was successfully added\"}", name);

    if (dprintf(client,
                "HTTP/1.1 303 See Other\r\n"
                "Location: /athletes\r\n"
                "Content-Length: %lu\r\n"
                "\r\n"
                "%s",
                strlen(response),
                response) < 0)
    {
        perror("Writing response to socket failed");
    };
}

void get_athletes(int client)
{

    char query[512];
    snprintf(query, sizeof(query),
             "SELECT * from athletes");
    PGresult *res = db_select(global_conn, query);

    if (!res)
    {
        send_400(client);
        return;
    }

    char *response = format_result(res);

    if (dprintf(client,
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: %lu\r\n"
                "\r\n"
                "%s",
                strlen(response), response) < 0)
    {
        perror("Writing response to socket failed");
    }
    free(response);
}

void update_athlete(int client, int id, char *body)
{
}
