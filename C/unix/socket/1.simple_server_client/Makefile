
ALL = server client

all: $(ALL)

clean:
	rm -f $(ALL) *~

server: server.c
	gcc server.c -o server
client: client.c
	gcc client.c -o client