FROM gcc:12

RUN apt-get update && apt-get install -y libpq-dev && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY *.c *.h ./

RUN gcc -Wall -Wextra -g -I/usr/include/postgresql -c *.c && \
    gcc -o server *.o -lpq

EXPOSE 3000

CMD sleep 3 && ./server