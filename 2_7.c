#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#define OK 0
#define ERROR_INVALID_ARGUMENTS -1
#define ERROR_MEMORY_ALLOCATION -2
#define ERROR_INVALID_NUMBER -3
#define ERROR_NO_Kaprekar_FOUND -4

int isKaprekar(int num);
int findKaprekarNumbers(char ***result, int base, int count, ...);

int main() {
    char **Kaprekar_numbers;
    
    int base = 10;
    int count = findKaprekarNumbers(&Kaprekar_numbers, base, 5, "1", "45", "55", "99", "297");

    if (count > 0) {
        printf("Kaprekar numbers in base %d:\n", base);
        for (int i = 0; i < count; i++) {
            printf("%s\n", Kaprekar_numbers[i]);
            free(Kaprekar_numbers[i]);
        }
        free(Kaprekar_numbers);
    } else if (count != OK) {
        switch (count) {
        case ERROR_MEMORY_ALLOCATION:
            printf("Error: Memory allocation failed.\n");
            break;
        case ERROR_NO_Kaprekar_FOUND:
            printf("Error: No Kaprekar numbers found.\n");
            break;
        default:
            printf("Error: Unknown error code %d.\n", count);
            break;
        }
    }
    
    base = 36;
    count = findKaprekarNumbers(&Kaprekar_numbers, base, 1, "19");

    if (count > 0) {
        printf("Kaprekar numbers in base %d:\n", base);
        for (int i = 0; i < count; i++) {
            printf("%s\n", Kaprekar_numbers[i]);
            free(Kaprekar_numbers[i]);
        }
        free(Kaprekar_numbers);
    } else if (count != OK) {
        switch (count) {
        case ERROR_MEMORY_ALLOCATION:
            printf("Error: Memory allocation failed.\n");
            break;
        case ERROR_NO_Kaprekar_FOUND:
            printf("Error: No Kaprekar numbers found.\n");
            break;
        default:
            printf("Error: Unknown error code %d.\n", count);
            break;
        }
    }

    return 0;
}

int isKaprekar(int num) {
    if (num < 0) return 0;

    long long square = (long long)num * num;
    int num_digits = 0, temp = num, i;
    
    while (temp > 0) {
        num_digits++;
        temp /= 10;
    }
    
    if (num == 0) {
        return 1;
    }

    long long divisor = 1;
    for (i = 0; i < num_digits; i++) {
        divisor *= 10;
    }
    
    long long left_part = square / divisor;
    long long right_part = square % divisor;
    
    return (left_part + right_part == num);
}

int findKaprekarNumbers(char ***result, int base, int count, ...) {
    va_list args;
    va_start(args, count);
    int i;

    *result = (char **)malloc(count * sizeof(char *));
    if (*result == NULL) {
        va_end(args);
        return ERROR_MEMORY_ALLOCATION;
    }

    int result_count = 0;
    for (i = 0; i < count; i++) {
        char *num_str = va_arg(args, char *); 
        
        char *endptr;
        
        if (num_str[0] == '-' || num_str[0] == '–') {
            continue;
        }

        long num = strtol(num_str, &endptr, base);
        
        if (*endptr != '\0' || num < 0) {
            continue;
        }

        if (isKaprekar(num)) {
            (*result)[result_count] = strdup(num_str);
            if ((*result)[result_count] == NULL) {
                for (int j = 0; j < result_count; j++) {
                    free((*result)[j]);
                }
                free(*result);
                va_end(args);
                return ERROR_MEMORY_ALLOCATION;
            }
            result_count++;
            
    }

    va_end(args);

    if (result_count == 0) {
        free(*result);
        *result = NULL;
        return ERROR_NO_Kaprekar_FOUND;
    }

    return result_count;
}
