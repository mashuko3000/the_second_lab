#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define OK 0
#define ERROR_INVALID_ARGUMENTS -1
#define ERROR_MEMORY_ALLOCATION -2
#define ERROR_INVALID_NUMBER -3
#define ERROR_NO_KAPREKAR_FOUND -4

int char_to_digit(char c, int base);
char digit_to_char(int digit);
char* pad_with_leading_zeros(const char *str, int length);
int add_in_base_columnwise(const char *num1, const char *num2, int base, char **result);
int sum_in_base_columnwise(int base, int num_count, const char *nums[], char **result);

int main() {
    int base = 6;  // for example, base 10 (decimal system)
    int num_count = 3;
    const char *numbers[] = {"0011000000000000000000000000000000000001", "0000000000000000000040000000000000000010", "5"};

    char *result;
    int status = sum_in_base_columnwise(base, num_count, numbers, &result);

    if (status == OK) {
        printf("sum of numbers in base %d: %s\n", base, result);  // print sum in the given base
        free(result);
    } else {
        switch (status) {
            case ERROR_MEMORY_ALLOCATION:
                printf("error: memory allocation failed.\n");
                break;
            case ERROR_INVALID_ARGUMENTS:
                printf("error: invalid base.\n");
                break;
            case ERROR_INVALID_NUMBER:
                printf("error: invalid character in number.\n");
                break;
            default:
                printf("error: unknown error code %d.\n", status);
                break;
        }
    }

    return 0;
}

int char_to_digit(char c, int base) {
    if (isdigit(c)) {
        return c - '0';
    } else if (isalpha(c)) {
        int digit = toupper(c) - 'A' + 10;
        if (digit >= base) {
            return ERROR_INVALID_NUMBER;  // invalid character for this base
        }
        return digit;
    }
    return ERROR_INVALID_NUMBER;  // invalid character
}

char digit_to_char(int digit) {
    if (digit < 10) {
        return digit + '0';
    } else {
        return digit - 10 + 'A';
    }
}

char* pad_with_leading_zeros(const char *str, int length) {
    int padding;
    int str_len = strlen(str);
    if (str_len >= length) {
        char *copy = strdup(str);
        if (copy==NULL) {
            return NULL;
        }
        return copy;
    }

    char *result = (char *)malloc(length + 1);
    if (result == NULL) {
        return NULL;  // memory allocation error
    }

    // add leading zeros
    padding = length - str_len;
    memset(result, '0', padding);  // fill with leading zeros
    strcpy(result + padding, str);  // copy the original string

    return result;
}

int add_in_base_columnwise(const char *num1, const char *num2, int base, char **result) {
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int sum_digit, i;
    int max_len = (len1 > len2) ? len1 : len2;
    int carry = 0;
    char *sum = (char *)malloc(max_len + 2); // for result with carry

    if (sum == NULL) {
        return ERROR_MEMORY_ALLOCATION;  // memory allocation error
    }

    int index = 0;
    while (len1 > 0 || len2 > 0 || carry > 0) {
        int digit1 = (len1 > 0) ? char_to_digit(num1[--len1], base) : 0;
        int digit2 = (len2 > 0) ? char_to_digit(num2[--len2], base) : 0;

        if (digit1 == ERROR_INVALID_NUMBER || digit2 == ERROR_INVALID_NUMBER) {
            free(sum);
            return ERROR_INVALID_NUMBER;  // invalid character
        }

        sum_digit = digit1 + digit2 + carry;
        carry = sum_digit / base;  // carry over
        sum[index++] = digit_to_char(sum_digit % base);  // remainder is added to result
    }

    sum[index] = '\0';

    // reverse the result string
    for (i = 0; i < index / 2; i++) {
        char temp = sum[i];
        sum[i] = sum[index - 1 - i];
        sum[index - 1 - i] = temp;
    }

    *result = sum;
    return OK;
}

int sum_in_base_columnwise(int base, int num_count, const char *nums[], char **result) {
    if (base < 2 || base > 36) {
        return ERROR_INVALID_ARGUMENTS;  // invalid base
    }

    // find the maximum length of the numbers
    int max_len = 0;
    int i, j, start;
    for (i = 0; i < num_count; i++) {
        int len = strlen(nums[i]);
        if (len > max_len) {
            max_len = len;
        }
    }

    // align all numbers to the maximum length by adding leading zeros
    char **padded_nums = (char **)malloc(num_count * sizeof(char *));
    if (padded_nums == NULL){
        return ERROR_MEMORY_ALLOCATION;
    }
    for (i = 0; i < num_count; i++) {
        padded_nums[i] = pad_with_leading_zeros(nums[i], max_len);
        if (padded_nums[i] == NULL) {
            for (j = 0; j < i; j++) {
                free(padded_nums[j]);
            }
            free(padded_nums);
            return ERROR_MEMORY_ALLOCATION;  // memory allocation error
        }
    }

    // now add the aligned numbers
    char *temp_result = strdup(padded_nums[0]);
    if (temp_result == NULL) {
        for (i = 0; i < num_count; i++) {
            free(padded_nums[i]);
        }
        free(padded_nums);
        return ERROR_MEMORY_ALLOCATION;  // memory allocation error
    }

    for (i = 1; i < num_count; i++) {
        char *sum_result;
        int status = add_in_base_columnwise(temp_result, padded_nums[i], base, &sum_result);
        if (status != OK) {
            free(temp_result);
            for (int i = 0; i < num_count; i++) {
                free(padded_nums[i]);
            }
            free(padded_nums);
            return status;  // error during addition
        }
        free(temp_result);
        temp_result = sum_result;
    }

    // removing leading zeros from the result
    start = 0;
    while (temp_result[start] == '0' && temp_result[start] != '\0') {
        start++;
    }

    // if all characters are zeros, return "0"
    if (temp_result[start] == '\0') {
        *result = strdup("0");
    } else {
        *result = strdup(temp_result + start);
    }

    free(temp_result);

    // free aligned numbers
    for (i = 0; i < num_count; i++) {
        free(padded_nums[i]);
    }
    free(padded_nums);

    return OK;
}
