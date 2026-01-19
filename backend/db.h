#include <libpq-fe.h>

PGconn *db_connect(void);
PGresult *db_select(PGconn *conn, char *query);
char *format_result(PGresult *res);
void print_result(PGresult *res);
int db_execute(PGconn *conn, char *query);

extern PGconn *global_conn;