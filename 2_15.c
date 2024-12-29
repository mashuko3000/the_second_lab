#include <stdio.h>
#include <stdlib.h>

typedef int tvalue;

#define ERROR_NULL_MATRIX 1
#define ERROR_NULL_SADDLE_POINTS_STORAGE 3
#define ERROR_NULL_SADDLE_POINTS_COUNT_STORAGE 4
#define ERROR_NULL_COMPARER 5
#define ERROR_MEMORY_ALLOCATION 6
#define ERROR_NULL_ROW 2
#define ERROR_EMPTY_MATRIX 7

int find_saddle_points(tvalue const * const *matrix, size_t matrix_rows_count, size_t matrix_columns_count, size_t ***found_saddle_points_storage, size_t *found_saddle_points_count_storage, int (*comparer)(tvalue const *, tvalue const *), int is_comparison_is_strict);

int int_comparer(tvalue const *a, tvalue const *b) {
    return (*a > *b) - (*a < *b);
}

int find_saddle_points(tvalue const * const *matrix, size_t matrix_rows_count, size_t matrix_columns_count, size_t ***found_saddle_points_storage, size_t *found_saddle_points_count_storage, int (*comparer)(tvalue const *, tvalue const *), int is_comparison_is_strict) {
    if (matrix == NULL) return ERROR_NULL_MATRIX;
    if (found_saddle_points_storage == NULL) return ERROR_NULL_SADDLE_POINTS_STORAGE;
    if (found_saddle_points_count_storage == NULL) return ERROR_NULL_SADDLE_POINTS_COUNT_STORAGE;
    if (comparer == NULL) return ERROR_NULL_COMPARER;
    if (matrix_rows_count == 0 || matrix_columns_count == 0) return ERROR_NULL_MATRIX;

    size_t **saddle_points = NULL;
    size_t count = 0;
    size_t **new_saddle_points = NULL;
    size_t k, i, j;
    size_t **temp_saddle_points = NULL;

    tvalue current_value;

    for (i = 0; i < matrix_rows_count; i++) {
        if (matrix[i] == NULL) return ERROR_NULL_ROW;

        for (j = 0; j < matrix_columns_count; j++) {
            current_value = matrix[i][j];
            int is_saddle_point = ERROR_NULL_MATRIX;

            for (k = 0; k < matrix_columns_count; k++) {
                if (k != j) {
                    int comp_result = comparer(&current_value, &matrix[i][k]);
                    
                    if (comp_result > 0 || (is_comparison_is_strict && comp_result == 0)) {
                        is_saddle_point = 0;
                        break;
                    }
                }
            }

            if (!is_saddle_point) continue;

            for (k = 0; k < matrix_rows_count; k++) {
                if (k != i) {
                    int comp_result = comparer(&current_value, &matrix[k][j]);
                    
                    if (comp_result < 0 || (is_comparison_is_strict && comp_result == 0)) {
                        is_saddle_point = 0;
                        break;
                    }
                }
            }
            if (is_saddle_point) {
                **temp_saddle_points = realloc(saddle_points, (count + 1) * sizeof(size_t *));
                if (temp_saddle_points == NULL) {
                    for (k = 0; k < count; k++) {
                        free(saddle_points[k]);
                    }
                    free(saddle_points);
                    return ERROR_MEMORY_ALLOCATION;
                }
            
            new_saddle_points = temp_saddle_points;
                saddle_points = new_saddle_points;
                saddle_points[count] = (size_t*)malloc(2 * sizeof(size_t));
                if (saddle_points[count] == NULL) {
                    for (k = 0; k < count; k++) {
                        free(saddle_points[k]);
                    }
                    free(saddle_points);
                    return ERROR_MEMORY_ALLOCATION;
                }
                saddle_points[count][0] = i;
                saddle_points[count][1] = j;
                count++;
            }
        }
    }

    *found_saddle_points_storage = saddle_points;
    *found_saddle_points_count_storage = count;

    return 0;
}

int main() {
    tvalue matrix_data[3][3] = {
        {7, 20, 5},
        {4, 5, 6},
        {7, 8, 9}
    };

    tvalue const *matrix[3] = {matrix_data[0], matrix_data[1], matrix_data[2]};
    size_t **saddle_points = NULL;
    size_t saddle_points_count = 0, i;

    int status = find_saddle_points(matrix, 3, 3, &saddle_points, &saddle_points_count, int_comparer, 0);
    if (status != 0) {
        printf("Error: %d\n", status);
        return status;
    }

    printf("Found %zu saddle point(s):\n", saddle_points_count);
    for (i = 0; i < saddle_points_count; i++) {
        printf("(%zu, %zu)\n", saddle_points[i][0] + 1, saddle_points[i][1] + 1);
        free(saddle_points[i]);
    }
    free(saddle_points);

    return 0;
}
