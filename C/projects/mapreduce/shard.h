#ifndef SHARD_H
#define SHARD_H


typedef struct shard {
    char *file_name;
    int start;
    int end;
} shard;

typedef struct vector {
    shard *arr;
    int size;
    int cap;
} vector;

void append(vector *v, shard x);

void delete_vector(vector *v);

void shard_file(vector *shards, char **file_names, int num_files);

int read_shard(shard s, char *buffer);

void print_shard_buffer(shard s);

#endif //SHARD_H
