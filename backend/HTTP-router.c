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

    if (strncmp(request, "OPTIONS", 7) == 0)
    {
        printf("options");
        fflush(stdout);
        if (dprintf(client, "HTTP/1.1 204 No Content\r\n"
                            "Access-Control-Allow-Origin: http://localhost:5173\r\n" // Må fjernes i prod
                            "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
                            "Access-Control-Allow-Headers: Content-Type\r\n"
                            "\r\n") < 0)
        {
            perror("Writing response to socket failed");
        };
        return;
    }

    else if (strcmp(method, "POST") == 0)
    {
        char *body = strstr(request, "\r\n\r\n");
        printf("post");
        fflush(stdout);

        if (!body)
        {
            send_400(client);
            return;
        }

        body += 4;

        if (strcmp(path, "/api/add_athlete") == 0)
        {
            handle_add_athlete(client, body);
        }
        else
        {
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

void send_response(int client, int status_code, const char *status_text, const char *body)
{
    if (dprintf(client,
                "HTTP/1.1 %d %s\r\n"
                "Content-Type: application/json\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
                "Access-Control-Allow-Headers: Content-Type\r\n"
                "Content-Length: %lu\r\n"
                "\r\n"
                "%s",
                status_code, status_text, strlen(body), body) < 0)
    {
        perror("Writing response to socket failed");
    };
}

void handle_status(int client)
{
    const char *json = "{ \"status\": \"ok\", \"service\": \"c-rest\" }";

    send_response(client, 200, "OK", json);
}

void send_404(int client)
{
    char *json = "{ \"error\": \"Not found\" }";

    send_response(client, 404, "Not Found", json);
}

void send_400(int client)
{
    const char *json = "{ \"error\": \"Bad request\" }";

    send_response(client, 400, "Bad Request", json);
}

void handle_add_athlete(int client, char *body)
{

    char name[256];

    sscanf(body, "name=%255s", name);

    char query[512];
    snprintf(query, sizeof(query),
             "INSERT INTO athletes (name, calories) VALUES ('%s', 0)",
             name);
    if (db_execute(global_conn, query) < 0)
    {
        send_400(client);
        return;
    }

    char response[512];
    snprintf(response, sizeof(response),
             "{\"message\": \"%s was successfully added\"}", name);

    send_response(client, 200, "OK", response);
}

void update_athlete(int client, int id, char *body)
{
    int calories;
    sscanf(body, "calories=%d", calories);

    char query[512];
    snprintf(query, sizeof(query),
             "UPDATE athletes SET calories = %d WHERE id = '%d'",
             id);
    if (db_execute(global_conn, query) < 0)
    {
        send_400(client);
        return;
    }

    char response[512];
    snprintf(response, sizeof(response),
             "{\"message\": \"Ahtlete with id: %d was successfully updated\"}", id);

    send_response(client, 200, "OK", response);
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

    send_response(client, 200, "OK", response);
    free(response);
}
