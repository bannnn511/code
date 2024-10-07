simple-counter: simple-counter.c util.h
	gcc -pthread -o simple-counter.out simple-counter.c

concurrent-counter: concurrent-counter.c util.h
	gcc -pthread -o concurrent-counter.out concurrent-counter.c

concurrent-queue: concurrent-queue.c util.h
	gcc -pthread -o concurrent-queue.out concurrent-queue.c

concurrent-linked-list: concurrent-linked-list.c
	gcc -pthread -o concurrent-linked-list.out concurrent-linked-list.c

hand-over-hand-linked-list: hand-over-hand-linked-list.c
	gcc -pthread -o hand-over-hand-linked-list.out hand-over-hand-linked-list.c

plot: plot.py
	chmod +x plot.py

all: simple-counter concurrent-counter concurrent-queue concurrent-linked-list hand-over-hand-linked-list plot

.PHONY: clean all

clean:
	rm -f ./*.out