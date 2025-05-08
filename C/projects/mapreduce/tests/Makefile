all: wordcount

wordcount-build:
	 gcc -g -Wall -Werror -pthread -o wordcount_test ../mapreduce.c ../task_queue.c ../shard.c ../kv.c wordcount.c
wordcount-run:
	./wordcount_test file1.txt
valgrind-wordcount:
	valgrind ./wordcount_test file1.txt --track-origin=yes --leak-check=full
