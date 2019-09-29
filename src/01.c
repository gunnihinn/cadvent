#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

off_t size(char* filename) {
    struct stat buffer;
    int status = stat(filename, &buffer);
    if (status) {
        return 0;
    }

    return buffer.st_size;
}

char *read_file(char* filename, off_t *bytes) {
    *bytes = size(filename);
    if (!*bytes) {
        fprintf(stderr, "Couldn't stat file\n");
        return NULL;
    }

    char* blob = malloc(sizeof(char) * *bytes);
    FILE *fh = fopen(filename, "r");
    fread(blob, sizeof(char), *bytes, fh);
    fclose(fh);

    return blob;
}

int32_t part_a(int *items, size_t count) {
    int32_t freq = 0;
    for (size_t i = 0; i < count; i++) {
        freq += *(items + i);
    }

    return freq;
}

int main(int argc, char* argv[]) {
    int ret = 0;

    off_t len = 0;
    char *blob = read_file(argv[1], &len);
    if (blob == NULL) {
        fprintf(stderr, "Couldn't read file\n");
        ret = 1;
        goto CLEANUP;
    }

    // Count newlines
    int count = 0;
    for (off_t i = 0; i < len; i++) {
        if (*(blob + i) == '\n') {
            count++;
        }
    }

    int *items = malloc(sizeof(int) * count);

    // Parse file contents
    char *token, *str;
    size_t i = 0;
    for (str = blob; ; str = NULL) {
        token = strtok(str, "\n");
        if (token == NULL) {
            break;
        }

        int freq = atoi(token);
        *(items + i) = freq;
        i++;
    }

    printf("Part A: %d\n", part_a(items, count));

CLEANUP:
    free(blob);

    return ret;
}
