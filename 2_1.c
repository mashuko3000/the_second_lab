#include <stdio.h>
#include <stdlib.h>

#define OK 0
#define ERROR_NULL_POINTER -1
#define ERROR_INVALID_NUMBER -2
#define ERROR_MEMORY_ALLOCATION -3
#define ERROR_INVALID_BASE -4

typedef enum {
    SUCCESS = 0,
    NULL_POINTER,
    INVALID_NUMBER,
    MEMORY_ALLOCATION_FAILED,
    INVALID_BASE,
    ERROR_COUNT
} ErrorCode;

const char* error_messages[ERROR_COUNT] = {
    "Operation completed successfully.",
    "Error: Null pointer passed.",
    "Error: Invalid number.",
    "Error: Failed to allocate memory.",
    "Error: Invalid base. Must be between 1 and 5."
};

// Прототипы функций
int add(int a, int b);
int subtract(int a, int b);
int multiply(int a, int b);
int divide(int a, int b);
int mod(int a, int b);
int convertToBase(int num, int r, char **result);
void reverseString(char *str, int length);
void handle_error(ErrorCode code);

int main() {
    int num = 1234;
    int r = 3;

    char *result = NULL;
    int status = convertToBase(num, r, &result);

    if (status != OK) {
        handle_error(status);
    } else {
        printf("number %d in base 2^%d system: %s\n", num, r, result);
        free(result);
    }

    return OK;
}

int add(int a, int b) {
    int mask;
    while (b != 0) {
        mask = a & b;
        a = a ^ b;
        b = mask << 1;
    }
    return a;
}

int subtract(int a, int b) {
    int mask;
    while (b != 0) {
        mask = (~a) & b;
        a = a ^ b;
        b = mask << 1;
    }
    return a;
}

int multiply(int a, int b) {
    int result = 0;
    while (b != 0) {
        if (b & 1) {
            result = add(result, a);
        }
        a <<= 1;
        b >>= 1;
    }
    return result;
}

int divide(int a, int b) {
    int result = 0;
    int shift;
    while (a >= b) {
        shift = 0;
        while (a >= (b << shift)) {
            shift = add(shift, 1);
        }
        shift = subtract(shift, 1);
        a = subtract(a, b << shift);
        result = add(result, 1 << shift);
    }
    return result;
}

int mod(int a, int b) {
    return subtract(a, multiply(divide(a, b), b));
}

int convertToBase(int num, int r, char **result) {
    if (result == NULL) {
        return NULL_POINTER;
    }
    if (r < 1 || r > 5) {
        return INVALID_BASE;
    }

    if (num < 0) {
        return INVALID_NUMBER;
    }

    const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int temp = num;
    int length = 0;
    int index;

    while (temp > 0) {
        length = add(length, 1);
        temp = divide(temp, 1 << r);   // делим на 2^r
    }

    if (length == 0) {
        length = 1;
    }

    *result = (char *)malloc(add(length, 1));
    if (*result == NULL) {
        return MEMORY_ALLOCATION_FAILED;
    }

    index = 0;
    while (num > 0) {
        (*result)[index] = digits[mod(num, 1 << r)];
        index = add(index, 1);
        num = divide(num, 1 << r);
    }

    (*result)[index] = '\0';

    reverseString(*result, index);

    return OK;
}

void reverseString(char *str, int length) {
    int i;
    char tempChar;
    for (i = 0; i < (length >> 1); i = add(i, 1)) {
        tempChar = str[i];
        str[i] = str[length - add(i, 1)];
        str[length - add(i, 1)] = tempChar;
    }
}

void handle_error(ErrorCode code) {
    if (code >= 0 && code < ERROR_COUNT) {
        printf("%s\n", error_messages[code]);
    } else {
        printf("Unknown error code.\n");
    }
}
