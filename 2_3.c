#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef enum {
    OK = 0,
    ERROR_MEMORY = 1,
    ERROR_OPEN_FILE = 2,
    ERROR_FREE_MEMORY = 3,
    XZ_CHO_DELAT = -1
} ErrorCode;

const char *error_messages[] = {
    "Operation successful",
    "Memory allocation error",
    "File open error",
    "Error freeing memory",
    "Unknown action error"
};

#define FREE_MEMORY 1
#define CLOSE_FILE 2

int release_resources(void *first_resource, ...) {
    va_list args;
    void *resource;
    int action;

    resource = first_resource;

    va_start(args, first_resource);

    while (resource != NULL) {
        action = va_arg(args, int);
        if (action == FREE_MEMORY) {
            free(resource);
        } else if (action == CLOSE_FILE) {
            fclose((FILE *)resource);
        } else {
            va_end(args);
            return XZ_CHO_DELAT;
        }

        resource = va_arg(args, void *);
    }

    va_end(args);
    return OK;
}

void handle_error(ErrorCode code) {
    if (code >= OK && code <= XZ_CHO_DELAT) {
        printf("Error: %s\n", error_messages[code]);
    } else {
        printf("Unknown error code: %d\n", code);
    }
}

int main() {
    int *dynamic_memory;
    FILE *file;
    int result;

    dynamic_memory = (int *)malloc(sizeof(int) * 10);
    if (!dynamic_memory) {
        handle_error(ERROR_MEMORY);  
        return ERROR_MEMORY;  
    }

    file = fopen("example.txt", "w");
    if (!file) {
        handle_error(ERROR_OPEN_FILE);
        free(dynamic_memory);
        return ERROR_OPEN_FILE;  
    }

    result = release_resources(dynamic_memory, FREE_MEMORY, file, CLOSE_FILE, NULL);
    if (result == XZ_CHO_DELAT) {
        handle_error(ERROR_FREE_MEMORY);
        return ERROR_FREE_MEMORY;
    }

    printf("Resources successfully released\n");
    return OK;
}
