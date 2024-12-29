#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {
    SUCCESS = 0,
    NULL_ITEMS,
    NULL_RESULT_SUBSETS,
    NULL_SUBSETS_COUNT,
    NULL_EQUALITY_COMPARER,
    ERROR_MEMORY_ALLOCATION,
    ERROR_UNIQUE_COLLECTION,
    ERROR_COUNT
} ErrorCode;

const char* error_messages[ERROR_COUNT] = {
    "Operation completed successfully.",
    "Error: Null pointer passed.",
    "Error: Null pointer passed.",
    "Error: Null pointer passed.",
    "Error: Null pointer passed.",
    "Error: Failed to allocate memory.",
    "Error: Invalid unique collection.",
    "Error: Invalid base. Must be between 1 and 5."
};

int int_inequality_comparer(int const *a, int const *b) {
    return *a != *b;
}

int subsets(int *items, size_t items_count, int ***result_subsets, size_t *result_subsets_count) {
    if (items == NULL) {
        return NULL_ITEMS;
    }
    if (result_subsets == NULL) {
        return NULL_RESULT_SUBSETS;
    }
    if (result_subsets_count == NULL) {
        return NULL_RESULT_SUBSETS;
    }

    size_t total_subsets = 1 << items_count;
    size_t subset_mask, i, k;
    int subset_size, j;

    *result_subsets = (int **)malloc(total_subsets * sizeof(int *));
    if (*result_subsets == NULL) {
        return ERROR_MEMORY_ALLOCATION;
    }

    *result_subsets_count = 0;
    
    for (subset_mask = 0; subset_mask < total_subsets; ++subset_mask) {
        subset_size = 0;

         for (i = 0; i < items_count; ++i) {
            if (subset_mask & (1 << i)) {
                subset_size++;
            }
        }
        
        (*result_subsets)[*result_subsets_count] = (int*) malloc((subset_size + 1) * sizeof(int));
        if ((*result_subsets)[*result_subsets_count] == NULL) {
            for (k = 0; k < *result_subsets_count; k++) {
                free((*result_subsets)[k]);
            }
              free(*result_subsets);
            *result_subsets = NULL;
            return ERROR_MEMORY_ALLOCATION;
        }
          (*result_subsets)[*result_subsets_count][0] = subset_size;
        
        j = 1;
        for (i = 0; i < items_count; ++i) {
            if (subset_mask & (1 << i)) {
                (*result_subsets)[*result_subsets_count][j] = items[i];
                 j++;
             }
        }
        (*result_subsets_count)++;
    }

    return SUCCESS;
}

void handle_error(ErrorCode code) {
    if (code >= 0 && code < ERROR_COUNT) {
        printf("%s\n", error_messages[code]);
    } else {
        printf("Unknown error code.\n");
    }
}

int main() {
    int items[] = {1, 2, 3};
    size_t items_count = 3;

    int **result_subsets = NULL;
    size_t result_subsets_count = 0, i;

    int status = subsets(items, items_count, &result_subsets, &result_subsets_count);

    if (status == 0) {
        printf("Finded %zu subsets:\n", result_subsets_count);

        for (size_t i = 0; i < result_subsets_count; ++i) {
            int subset_size = result_subsets[i][0];
            printf("{ ");
            for (int j = 1; j <= subset_size; ++j) {
                printf("%d ", result_subsets[i][j]);
            }
            printf("}\n");
        }

        int i;
        if(result_subsets == NULL) return;
        for (i = 0; i < result_subsets_count; i++) {
            free(result_subsets[i]);
        }

    } else {
        handle_error(status);
    }

    return SUCCESS;
}