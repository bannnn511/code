CC = gcc
CFLAGS = -Wall -Wextra -std=c23 -g -D_GNU_SOURCE
TARGETS = server client

all: $(TARGETS)

server: server.c io.c
	$(CC) $(CFLAGS) -o server server.c

client: client.c io.c
	$(CC) $(CFLAGS) -o client client.c

clean:
	rm -f $(TARGETS)

test: $(TARGETS)
	@echo "Starting server in background..."
	./server 8080 &
	@sleep 1
	@echo "Testing client..."
	./client localhost 8080 "test message"
	@pkill server || true

simulate-loss:
	sudo tc qdisc add dev lo root netem loss $(LOSS)%

simulate-delay:
	sudo tc qdisc add dev lo root netem delay 100ms loss $(LOSS)%

cleanup-simulation:
	sudo tc qdisc del dev lo root

check-simulation:
	tc qdisc show dev lo

.PHONY: all clean test simulate-loss simulate-delay cleanup-simulation check-simulation