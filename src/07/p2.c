#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#define MAX_CHARS 64

struct dir_t {
    char *name;
    size_t size;
    struct dir_t *parent;
    size_t dir_count;
    struct dir_t **dirs;
    size_t file_count;
    size_t *files;
};

void free_dirs(struct dir_t* dir) {
    free(dir->name);
    free(dir->files);

    for (size_t i = 0; i < dir->dir_count; i++) {
        free_dirs(dir->dirs[i]);
    }
    free(dir->dirs);
    free(dir);
}

void print_dir(struct dir_t *dir, size_t depth) {
    size_t i;

    for (i = 0; i < depth; i++){
        printf("\t");
    }
    printf("%s (dirs %lu files %lu)\n", dir->name, dir->dir_count, dir->file_count);
    for (i = 0; i < dir->dir_count; i++){
        print_dir(dir->dirs[i], depth + 1);
    }
}

void print_filesystem(struct dir_t *root) {
    printf("%s (dirs %lu files %lu)\n", root->name, root->dir_count, root->file_count);
    for (size_t i = 0; i < root->dir_count; i++){
        print_dir(root->dirs[i], 1);
    }
}

char *get_dir_name(char * buffer, size_t start) {
    size_t length;
    char *substring;

    for (length = 0; length < MAX_CHARS; length++) {
        if (!isalpha(buffer[start + length])) {
            break;
        }
    }

    substring = (char *) calloc(length + 1, sizeof(char));
    memcpy(substring, &buffer[start], length);
    substring[length] = '\0';

    return substring;
}

struct dir_t *find_dir_with_name(struct dir_t *current, char *name) {
    for (size_t i = 0; i < current->dir_count; i++) {
        if (strcmp(current->dirs[i]->name, name) == 0) {
            return current->dirs[i];
        }
    }

    exit(1);
}

void populate_filesystem(struct dir_t *current) {
    char buffer[MAX_CHARS] = {0, };
    struct dir_t *temp_dir;
    char *temp_string;
    fgets(buffer, MAX_CHARS, stdin); // skip first line

    while(fgets(buffer, MAX_CHARS, stdin) != NULL) {
        if (buffer[0] == '$') {
            if (buffer[2] == 'l') {
                continue;
            } else if (buffer[5] == '.') {
                current = current->parent;
                continue;
            }
            temp_string = get_dir_name(buffer, 5);
            current = find_dir_with_name(current, temp_string);
            free(temp_string);
        } else if (isdigit(buffer[0])) {
            current->file_count++;
            current->files = (size_t *) realloc(current->files, current->file_count * sizeof(size_t));
            current->files[current->file_count - 1] = strtoul(buffer, NULL, 10);
        } else {
            current->dir_count++;
            current->dirs = (struct dir_t **) realloc(current->dirs, current->dir_count * sizeof(struct dir_t *));
            temp_dir = (struct dir_t *) calloc(1, sizeof(struct dir_t));
            temp_dir->name = get_dir_name(buffer, 4);
            temp_dir->parent = current;
            current->dirs[current->dir_count - 1] = temp_dir;
        }
    }
}

size_t calculate_sizes(struct dir_t *current) {
    size_t i;
    size_t dir_size = 0;

    for (i = 0; i < current->file_count; i++) {
        dir_size += current->files[i];
    }

    for ( i = 0; i < current->dir_count; i++) {
        dir_size += calculate_sizes(current->dirs[i]);
    }

    current->size = dir_size;

    return dir_size;
}

size_t find_smallest_freeable_dir(struct dir_t *current, size_t size_to_free, size_t *freeable) {
    size_t i;
    size_t dir_size = 0;

    for ( i = 0; i < current->dir_count; i++) {
        dir_size += find_smallest_freeable_dir(current->dirs[i], size_to_free, freeable);
    }

    if (current->size < *freeable && current->size >= size_to_free) {
        *freeable = current->size;
    }

    return dir_size;
}

int main(void) {
    struct dir_t *root = (struct dir_t *) calloc(1, sizeof(struct dir_t));
    root->name = (char *) calloc(2, sizeof(char));
    root->name[0] = '/';

    populate_filesystem(root);
    size_t total_size = calculate_sizes(root);
    size_t size_to_free = 30000000 - (70000000 - total_size);
    size_t smallest_freeable_size = ULONG_MAX;
    find_smallest_freeable_dir(root, size_to_free, &smallest_freeable_size);
    printf("the smallest freeable dir has a size of %lu\n", smallest_freeable_size);
    free_dirs(root);
}