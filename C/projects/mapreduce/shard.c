#include "shard.h"
#include <stdlib.h>
#include <sys/stat.h>
#include  <unistd.h>
#include <stdio.h>

int shard_size = 16;

void append(vector *v, const shard x) {
    if (v == NULL) {
        return;
    }
    if (v->arr == NULL) {
        v->arr = calloc(10, sizeof(shard));
        v->cap = 10;
        v->size = 0;
    }
    if (v->size == v->cap) {
        v->cap *= 2;
        v->arr = realloc(v->arr, sizeof(shard) * v->cap);
    }
    v->arr[v->size++] = x;
}


void delete_vector(vector *v) {
    if (v == NULL) {
        return;
    }

    free(v->arr);
    free(v);
}

off_t get_file_size(char *file_name) {
    struct stat st;
    if (stat(file_name, &st) == -1) {
        return -1;
    }

    return st.st_size;
}

void print_shard_buffer(const shard s) {
    FILE *f = fopen(s.file_name, "r");
    if (f == NULL) {
        perror("open");
        return;
    }

    fseek(f, s.start, SEEK_SET);
    char buffer[s.end - s.start + 1];
    size_t bytes_read = fread(buffer, 1, s.end - s.start, f);
    buffer[bytes_read] = '\0';
    printf("Shard content: %s\n", buffer);
    fclose(f);
}


void shard_file(vector *shards, char **file_names, int num_files) {
    for (int i = 0; i < num_files; i++) {
        off_t file_size = get_file_size(file_names[i]);
        if (file_size == -1) {
            continue;
        }

        FILE *fp = fopen(file_names[i], "r");
        if (!fp) {
            continue;
        }

        size_t total_size = 0;
        while (total_size < (size_t) file_size) {
            shard s = {
                .file_name = file_names[i],
                .start = total_size,
                .end = total_size + shard_size
            };

            if ((size_t) s.end >= (size_t) file_size) {
                s.end = file_size;
                total_size = file_size;
            } else {
                if (fseek(fp, s.end, SEEK_SET) != 0) {
                    continue;
                }

                int c;
                while ((c = fgetc(fp)) != EOF && c != ' ' && c != '\n') {
                    s.end++;
                }
                total_size = s.end + 1;
            }

            append(shards, s);
        }

        fclose(fp);
    }
}

int read_shard(shard s, char *buffer) {
    FILE *f = fopen(s.file_name, "r");
    if (f == NULL) {
        return -1;
    }

    fseek(f, s.start, SEEK_SET);
    size_t bytes_read = fread(buffer, 1, s.end - s.start, f);
    buffer[bytes_read] = '\0';
    fclose(f);

    return 0;
}
