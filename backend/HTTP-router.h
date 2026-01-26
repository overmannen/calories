#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void handle_request(int client, char *request);

void handle_status(int client);

void send_404(int client);

void send_400(int client);

void handle_add_athlete(int client, char *request);

void get_athletes(int client);

void update_athlete(int client, int id, char *body);

void send_response(int client, int status_code, const char *status_text, const char *body);

void delete_athlete(int client, int id);