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

struct tree {
    int val;
    struct tree *left;
    struct tree *right;
};

struct tree *tree_init(int val) {
    struct tree *t = malloc(sizeof(struct tree));

    t->val = val;
    t->left = NULL;
    t->right = NULL;

    return t;
}

int tree_contains(struct tree *t, int val) {
    if (t == NULL) {
        return 0;
    }

    if (t->val == val) {
        return 1;
    }

    if (t->val < val) {
        return tree_contains(t->left, val);
    } else {
        return tree_contains(t->right, val);
    }
}

void tree_add(struct tree *t, int val) {
    if (t->val < val) {
        if (t->left == NULL) {
            struct tree *c = tree_init(val);
            t->left = c;
        } else {
            tree_add(t->left, val);
        }
    } else {
        if (t->right == NULL) {
            struct tree *c = tree_init(val);
            t->right = c;
        } else {
            tree_add(t->right, val);
        }
    }
}

int part_b(int *items, size_t count) {
    int freq = 0;
    struct tree *map = tree_init(freq);

    for (size_t i = 0;; i++) {
        freq += *(items + (i % count));

        if (tree_contains(map, freq)) {
            return freq;
        } else {
            tree_add(map, freq);
        }
    }

    return 0;
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
    for (str = blob;; str = NULL) {
        token = strtok(str, "\n");
        if (token == NULL) {
            break;
        }

        int freq = atoi(token);
        *(items + i) = freq;
        i++;
    }

    printf("Part A: %d\n", part_a(items, count));
    printf("Part B: %d\n", part_b(items, count));

CLEANUP:
    free(blob);

    return ret;
}
