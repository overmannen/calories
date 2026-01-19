void handle_request(int client, char *request);

void handle_status(int client);

void send_404(int client);

void send_400(int client);

void handle_add_athlete(int client, char *request);

void get_athletes(int client);

void update_athlete(int client, int id, char *body);