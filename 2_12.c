#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#define OK 0
#define NOT_STRING 1
#define NOT_RESULT 2
#define NULL_POSITION 3
#define NULL_COUNT 4
#define MEMORY_ERROR 5

int substr(char* to_find, int case_sensitive, char*** results, int*** positions, size_t* results_count, ...);

int main() {
    char** results;
    int** positions;
    size_t results_count, i;
    int status, j;

    status = substr("luntik", 0, &results, &positions, &results_count, "This is my luntik.", "Luntik is bizarre entity.", "Moon is their birthplace.", NULL);

    if (status == OK) {
        printf("Substring found in %zu strings.\n", results_count);
        for (i = 0; i < results_count; i++) {
            printf("String: %s\n", results[i]);
            printf("Positions: ");
            j = 0;
            while (positions[i][j] != -1) {
                printf("%d ", positions[i][j]);
                j++;
            }
            printf("\n");
        }

        for (i = 0; i < results_count; i++) {
            free(positions[i]);
            free(results[i]);
        }
        free(results);
        free(positions);
    } else {
        printf("Error: %d\n", status);
    }
    return 0;
}

int substr(char* to_find, int case_sensitive, char*** results, int*** positions, size_t* results_count, ...) {
    unsigned int len_to_find = strlen(to_find);
    char * to_find_lower = NULL;
    va_list args;
    int size_p, k;
    char* current_str;
    size_t i, len_current_str, len_substr, len_str;
    int** temp_positions = NULL;
    char** temp_results = NULL;

    if (results == NULL) return NOT_RESULT;
    if (positions == NULL) return NULL_POSITION;
    if (results_count == NULL) return NULL_COUNT;
    if (to_find == NULL) return NOT_STRING;

    to_find_lower = strdup(to_find);
    if (to_find_lower == NULL) {
        return MEMORY_ERROR;
    }

    if (case_sensitive == 0) {
        for (i = 0; i < len_to_find; i++) {
            to_find_lower[i] = tolower(to_find[i]);
        }
    }

    va_start(args, results_count);

    int size_p = 10;
    *positions = (int**)malloc(sizeof(int*) * size_p);
    *results = (char**)malloc(sizeof(char*) * size_p);
    if (*positions == NULL || *results == NULL) {
        free(to_find_lower);
        va_end(args);
        if (*positions) free(*positions);
        if (*results) free(*results);
        return MEMORY_ERROR;
    }

    *results_count = 0;

    char* current_str;
    while ((current_str = va_arg(args, char*)) != NULL) {
        int k = 0;
        int size = 10;
        int* temp_buffer = (int*)malloc(sizeof(int) * size);
        if (temp_buffer == NULL) {
            va_end(args);
            free(to_find_lower);
            for (i = 0; i < *results_count; i++) {
                free((*positions)[i]);
                free((*results)[i]);
            }
            free(*positions);
            free(*results);
            return MEMORY_ERROR;
        }

        len_current_str = strlen(current_str);
        char* current_str_lower = strdup(current_str);
        if (current_str_lower == NULL) {
            va_end(args);
            free(temp_buffer);
            free(to_find_lower);
            for (i = 0; i < *results_count; i++) {
                free((*positions)[i]);
                free((*results)[i]);
            }
            free(*positions);
            free(*results);
            return MEMORY_ERROR;
        }

        if (case_sensitive == 0) {
            for (i = 0; i < len_current_str; i++) {
                current_str_lower[i] = tolower(current_str[i]);
            }
        }

        len_substr = strlen(to_find_lower);
        len_str = strlen(current_str_lower);

        for (i = 0; i <= len_str - len_substr; i++) {
            if (strncmp(&current_str_lower[i], to_find_lower, len_substr) == 0) {
                if (k >= size) {
                    size *= 2;
                    int* temp_realloc = (int*)realloc(temp_buffer, sizeof(int) * size);
                    if (temp_realloc == NULL) {
                        free(temp_buffer);
                        free(current_str_lower);
                        va_end(args);
                        free(to_find_lower);
                        for (i = 0; i < *results_count; i++) {
                            free((*positions)[i]);
                            free((*results)[i]);
                        }
                        free(*positions);
                        free(*results);
                        return MEMORY_ERROR;
                    }
                    temp_buffer = temp_realloc;
                }
                temp_buffer[k++] = i;
            }
        }

        if (k > 0) {
            temp_buffer[k] = -1;
            (*positions)[*results_count] = temp_buffer;
            (*results)[*results_count] = strdup(current_str);
            if ((*results)[*results_count] == NULL) {
                free(temp_buffer);
                free(current_str_lower);
                va_end(args);
                free(to_find_lower);
                for (i = 0; i < *results_count; i++) {
                    free((*positions)[i]);
                    free((*results)[i]);
                }
                free(*positions);
                free(*results);
                return MEMORY_ERROR;
            }

            (*results_count)++;
        } else {
            free(temp_buffer);
        }

        free(current_str_lower);

        if (*results_count >= size_p) {
            size_p *= 2;
            temp_positions = (int**)realloc(*positions, sizeof(int*) * size_p);
            temp_results = (char**)realloc(*results, sizeof(char*) * size_p);

            if (temp_positions == NULL || temp_results == NULL) {
                free(to_find_lower);
                va_end(args);
                for (size_t i = 0; i < *results_count; i++) {
                    free((*positions)[i]);
                    free((*results)[i]);
                }
                free(*positions);
                free(*results);
                return MEMORY_ERROR;
            }

            *positions = temp_positions;
            *results = temp_results;
        }
    }

    va_end(args);
    free(to_find_lower);
    return OK;
}
