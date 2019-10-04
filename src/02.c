#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

off_t size(char *filename) {
    struct stat buffer;
    int status = stat(filename, &buffer);
    if (status) {
        return 0;
    }

    return buffer.st_size;
}

char *read_file(char *filename, off_t *bytes) {
    *bytes = size(filename);
    if (!*bytes) {
        fprintf(stderr, "Couldn't stat file\n");
        return NULL;
    }

    char *blob = malloc(sizeof(char) * *bytes);
    if (blob == NULL) {
        return blob;
    }

    FILE *fh = fopen(filename, "r");
    fread(blob, sizeof(char), *bytes, fh);
    fclose(fh);

    return blob;
}

int newline_count(char *blob, int len) {
    int count = 0;
    for (off_t i = 0; i < len; i++) {
        if (*(blob + i) == '\n') {
            count++;
        }
    }

    return count;
}

const uint8_t HAS_PAIR = 1 << 0;
const uint8_t HAS_TRIPLE = 1 << 1;

uint8_t count_repeats(char *line, size_t len) {
    uint8_t mask = 0;

    char letters[26] = {0};
    for (size_t i = 0; i < len; i++) {
        letters[*(line + i) - 'a']++;
    }

    for (size_t i = 0; i < 26; i++) {
        if (letters[i] == 2) {
            mask |= HAS_PAIR;
        } else if (letters[i] == 3) {
            mask |= HAS_TRIPLE;
        }
    }

    return mask;
}

uint32_t part_a(char **items, int count) {
    int32_t two = 0;
    int32_t three = 0;
    for (size_t i = 0; i < count; i++) {
        char *line = *(items + i);
        uint8_t count = count_repeats(line, 27);

        if (count & HAS_PAIR) {
            two++;
        }
        if (count & HAS_TRIPLE) {
            three++;
        }
    }

    return two * three;
}

// Input: strings left and right, buffer to put common substring into.
// Output: length of common substring.
uint8_t common(char *left, char *right, char *common) {
    uint8_t len = 0;

    for (size_t i = 0; i < 27; i++) {
        if (*left == *right) {
            *common = *left;
            common++;
            len++;
        }

        left++;
        right++;
    }

    *common = '\0';

    return len;
}

char *part_b(char **items, int count) {
    char *buf = malloc(28 * sizeof(char));
    char *zero = calloc(sizeof(char), 28);

    for (size_t i = 0; i < count; i++) {
        for (size_t j = i + 1; j < count; j++) {
            memcpy(buf, zero, 28);
            uint8_t len = common(*(items + i), *(items + j), buf);
            if (len == 26) {
                free(zero);
                return buf;
            }
        }
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    int ret = 0;

    off_t len = 0;
    char *blob = read_file(argv[1], &len);
    if (blob == NULL) {
        fprintf(stderr, "Couldn't read file\n");
        ret = 1;
        goto CLEANUP;
    }

    int count = newline_count(blob, len);
    char **items = malloc(sizeof(char *) * count);
    if (items == NULL) {
        fprintf(stderr, "Couldn't allocate memory for items\n");
        return 1;
    }

    // Parse file contents
    char *token, *str;
    size_t i = 0;
    for (str = blob;; str = NULL) {
        token = strtok(str, "\n");
        if (token == NULL) {
            break;
        }

        *(items + i) = token;
        i++;
    }

    printf("Part A: %d\n", part_a(items, count));

    char *buf = part_b(items, count);
    if (buf) {
        printf("Part B: %s\n", buf);
    }

CLEANUP:
    free(blob);
    free(buf);

    return ret;
}
