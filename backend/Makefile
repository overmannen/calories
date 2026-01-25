CC = gcc
CFLAGS = -Wall -Wextra -g -I/opt/homebrew/opt/libpq/include
LDFLAGS = -L/opt/homebrew/opt/libpq/lib
LDLIBS = -lpq

TARGET = server

OBJS = test_server.o Server.o HTTP-router.o db.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

# Docker
up:
	docker-compose up -d

down:
	docker-compose down

logs:
	docker-compose logs -f

restart: down all up

.PHONY: all clean run up down logs restart