#include <stdio.h>
#include <stdlib.h>

#define OK 0
#define ERROR_NULL_ITEMS 1
#define ERROR_NULL_RESULT_PERMUTATIONS 2
#define ERROR_NULL_RESULT_PERMUTATIONS_COUNT 3
#define ERROR_NULL_EQUALITY_COMPARER 4
#define ERROR_MEMORY_ALLOCATION 5
#define ERROR_UNIQUE_ELEMENTS 6

int int_equality_comparer(int const *a, int const *b) {
    return *a == *b;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void generate_permutations(int *items, size_t items_count, int **result_permutations, size_t *index, int start) {
    size_t i;
    if (start == items_count) {
        for (i = 0; i < items_count; i++) {
            result_permutations[*index][i] = items[i];
        }
        (*index)++;
        return;
    }

    for (i = start; i < items_count; i++) {
        swap(&items[start], &items[i]);
        generate_permutations(items, items_count, result_permutations, index, start + 1);
        swap(&items[start], &items[i]);
    }
}

int permutations(int *items, size_t items_count, int ***result_permutations, size_t *result_permutations_count,
                 int (*equality_comparer)(int const *, int const *)) {
    size_t i, j, total_permutations;
    int status;
    if (items == NULL) {
        return ERROR_NULL_ITEMS;
    }
    if (result_permutations == NULL) {
        return ERROR_NULL_RESULT_PERMUTATIONS;
    }
    if (result_permutations_count == NULL) {
        return ERROR_NULL_RESULT_PERMUTATIONS_COUNT;
    }
    if (equality_comparer == NULL) {
        return ERROR_UNIQUE_ELEMENTS;
    }

    for (i = 0; i < items_count; ++i) {
        for (j = i + 1; j < items_count; ++j) {
            if (equality_comparer(&items[i], &items[j])) {
                return ERROR_UNIQUE_ELEMENTS;
            }
        }
    }

    total_permutations = 1;
    for (i = 1; i <= items_count; i++) {
        total_permutations *= i;
    }

    *result_permutations = (int **)malloc(total_permutations * sizeof(int *));
    if (*result_permutations == NULL) {
        return ERROR_MEMORY_ALLOCATION;
    }

    for (i = 0; i < total_permutations; ++i) {
        (*result_permutations)[i] = (int *)malloc(items_count * sizeof(int));
        if ((*result_permutations)[i] == NULL) {
            return ERROR_MEMORY_ALLOCATION;
        }
    }

    *result_permutations_count = 0;
    generate_permutations(items, items_count, *result_permutations, result_permutations_count, 0);

    return OK;
}

void free_permutations(int **permutations, size_t total_permutations) {
    size_t i;
    for (i = 0; i < total_permutations; ++i) {
        free(permutations[i]);
    }
    free(permutations);
}

int main() {
    int items[] = {1, 2, 3, 3};
    size_t items_count = 4;
    int status;
    int **result_permutations;
    size_t result_permutations_count;

    status = permutations(items, items_count, &result_permutations, &result_permutations_count, int_equality_comparer);

    if (status == 0) {
        printf("Найдено %zu перестановок:\n", result_permutations_count);

        for (size_t i = 0; i < result_permutations_count; ++i) {
            printf("{ ");
            for (size_t j = 0; j < items_count; ++j) {
                printf("%d ", result_permutations[i][j]);
            }
            printf("}\n");
        }

        free_permutations(result_permutations, result_permutations_count);
    } else {
        printf("Произошла ошибка: %d\n", status);
    }

    return OK;
}
