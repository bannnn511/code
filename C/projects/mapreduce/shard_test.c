#include "shard.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
    vector *s = calloc(sizeof(shard), 1);
    char *file = "./tests/file1.txt";
    shard_file(s, &file, 1);

    for (int i = 0; i < s->size; i++) {
        printf("shard[%d] %s, %d, %d\n", i, s->arr[i].file_name, s->arr[i].start, s->arr[i].end);
        print_shard_buffer(s->arr[i]);
    }

    free(s);
    return 0;
}
