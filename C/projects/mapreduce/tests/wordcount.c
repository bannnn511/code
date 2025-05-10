#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "../mapreduce.h"

// void Map(char *file_name) {
//     FILE *fp = fopen(file_name, "r");
//     assert(fp != NULL);
//
//     char *line = NULL;
//     size_t size = 0;
//     while (getline(&line, &size, fp) != -1) {
//         char *token, *dummy = line;
//         while ((token = strsep(&dummy, " \t\n")) != NULL) {
//             if (strlen(token) > 0) {
//                 MR_Emit(token, "1");
//             }
//         }
//     }
//
//     free(line);
//     fclose(fp);
// }

// new version of map to read from buffer instead of file to be more like mapreduce style
void Map(char *buffer) {
    char *line = strdup(buffer);  // Create a copy since strsep modifies the string
    if (line == NULL) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }
    char *save_ptr = line;

    char *token;
    while ((token = strsep(&save_ptr, " \t\n")) != NULL) {
        if (strlen(token) > 0) {
            MR_Emit(token, "1");
        }
    }

    free(line);
}

void Reduce(char *key, Getter get_next, int partition_number) {
    int count = 0;
    char *value;
    while ((value = get_next(key, partition_number)) != NULL) {
        count += atoi(value);
    }
    printf("%s: %d\n", key, count);
}

/**
 * ./wordcount file1.txt file2.txt
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    MR_Run(argc, argv, Map, 20, Reduce, 20, MR_DefaultHashPartition);
}
