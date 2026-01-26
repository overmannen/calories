#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"

PGconn *global_conn = NULL;

PGconn *db_connect(void)
{
    const char *host = getenv("DB_HOST") ?: "localhost";
    const char *port = getenv("DB_PORT") ?: "5432";
    const char *dbname = getenv("DB_NAME") ?: "calories";
    const char *user = getenv("DB_USER") ?: "jesper";
    const char *password = getenv("DB_PASSWORD") ?: "123";

    char conninfo[512];
    snprintf(conninfo, sizeof(conninfo),
             "host=%s port=%s dbname=%s user=%s password=%s",
             host, port, dbname, user, password);

    PGconn *conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK)
    {
        printf("Error while connecting to the database server: %s\n", PQerrorMessage(conn));
        PQfinish(conn);

        return NULL;
    }

    printf("Connection Established\n");
    printf("Port: %s\n", PQport(conn));
    printf("Host: %s\n", PQhost(conn));
    printf("DBName: %s\n", PQdb(conn));

    return conn;
}

PGresult *db_select(PGconn *conn, char *query)
{

    PGresult *res = PQexec(conn, query);

    ExecStatusType status = PQresultStatus(res);

    printf("Query Status: %s\n", PQresStatus(status));

    if (status != PGRES_TUPLES_OK)
    {
        printf("Error while executing the query: %s\n", PQerrorMessage(conn));

        PQclear(res);

        PQfinish(conn);

        return NULL;
    }

    printf("Query exucted successfully\n");

    return res;
}

void print_result(PGresult *res)
{
    int rows = PQntuples(res);
    int cols = PQnfields(res);

    printf("Number of rows: %d\n", rows);
    printf("Number of columns: %d\n", cols);

    for (int i = 0; i < rows; i++)
    {
        printf("%s\n", PQfname(res, i));
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%s\t", PQgetvalue(res, i, j));
        }
        printf("\n");
    }

    PQclear(res);
}

char *format_result(PGresult *res)
{
    char *response = malloc(4096);

    if (!response)
    {
        PQclear(res);
        return NULL;
    }
    char *ptr = response;
    memset(response, 0, 4096);

    int rows = PQntuples(res);

    ptr += sprintf(ptr, "{\"athletes\": [");

    for (int i = 0; i < rows; i++)
    {
        int id = atoi(PQgetvalue(res, i, 0));
        char *name = PQgetvalue(res, i, 1);
        int calories = atoi(PQgetvalue(res, i, 2));

        ptr += sprintf(ptr,
                       "{\"id\": %d, \"name\": \"%s\", \"calories\": %d}",
                       id, name, calories);

        if (i < rows - 1)
            ptr += sprintf(ptr, ",");
    }

    ptr += sprintf(ptr, "]}");
    PQclear(res);

    return response;
}

int db_execute(PGconn *conn, char *query)
{
    PGresult *res = PQexec(conn, query);

    ExecStatusType status = PQresultStatus(res);

    printf("Query Status: %s\n", PQresStatus(status));

    if (status != PGRES_COMMAND_OK)
    {
        printf("Error while executing the query: %s\n", PQerrorMessage(conn));

        PQclear(res);

        PQfinish(conn);

        return -1;
    }

    printf("Query exucted successfully\n");
    int rows = atoi(PQcmdTuples(res));
    PQclear(res);

    return rows;
}
