all: wordcount

wordcount-build:
	 gcc -g -Wall -Werror -pthread -o wordcount_test ../mapreduce.c ../task_queue.c ../shard.c ../kv.c ../threadpool.c wordcount.c ../heap.c
wordcount-run:
	./wordcount_test file1.txt
valgrind-wordcount:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./wordcount_test file1.txt
