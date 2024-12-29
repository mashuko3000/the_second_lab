#include <stdio.h>
#include <stdlib.h>

#define OK 0
#define ERROR_NULL_POINTER -1
#define ERROR_INVALID_NUMBER -2
#define ERROR_MEMORY_ALLOCATION -3
#define ERROR_INVALID_BASE -4

// function prototypes
int add(int a, int b);
int subtract(int a, int b);
int multiply(int a, int b);
int divide(int a, int b);
int mod(int a, int b);
int convertToBase(int num, int r, char **result);
void reverseString(char *str, int length);

int main() {
    int num = 1234;
    int r = 3;

    char *result = NULL;
    int status = convertToBase(num, r, &result);

    if (status != OK) {
        if (status == ERROR_INVALID_BASE) {
            printf("error: invalid base\n");
        } else if (status == ERROR_INVALID_NUMBER) {
            printf("error: invalid number\n");
        } else if (status == ERROR_MEMORY_ALLOCATION) {
            printf("error: failed to allocate memory for result\n");
        } else {
            printf("unknown error: %d\n", status);
        }
    } else {
        printf("number %d in base 2^%d system: %s\n", num, r, result);
        free(result);
    }

    return OK;
}

// function for bitwise addition
int add(int a, int b) {
    int mask;
    while (b != 0) {
        mask = a & b;    // find mask
        a = a ^ b;             // add without mask
        b = mask << 1;        // shift
    }
    return a;
}

// function for bitwise subtraction
int subtract(int a, int b) {
    int mask;
    while (b != 0) {
        mask = (~a) & b;  // find mask
        a = a ^ b;              // subtract without mask
        b = mask << 1;        // shift mask
    }
    return a;
}

// function for bitwise multiplication
int multiply(int a, int b) {
    int result = 0;
    while (b != 0) {
        if (b & 1) {           // check if odd
            result = add(result, a);  // if odd, add a to result
        }
        a <<= 1;               // multiply a by 2 
        b >>= 1;               // divide b by 2
    }
    return result;
}

// function for bitwise division (division by 2^r)
int divide(int a, int b) {
    int result = 0;
    int shift;
    while (a >= b) {
        shift = 0;
        while (a >= (b << shift)) {  // find maximum shift (a >= b << shift)
            shift = add(shift, 1);
        }
        shift = subtract(shift, 1);  // revert to last shift
        a = subtract(a, b << shift); // subtract shifted b from a
        result = add(result, 1 << shift);  // add shift to result
    }
    return result;
}

// function for bitwise modulo
int mod(int a, int b) {
    return subtract(a, multiply(divide(a, b), b));  // a - (a / b) * b
}

int convertToBase(int num, int r, char **result) {
    if (result == NULL) {
        return ERROR_NULL_POINTER;
    }
    if (r < 1 || r > 5) {
        return ERROR_INVALID_BASE;
    }

    if (num < 0) {
        return ERROR_INVALID_NUMBER;
    }

    const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int temp = num;
    int length = 0;
    int index;

    // count digits after conversion
    while (temp > 0) {
        length = add(length, 1);
        temp = divide(temp, 1 << r);   // divide by 2^r
    }

    // if number is 0
    if (length == 0) {
        length = 1;
    }

    // allocate memory for result array (including '\0' character)
    *result = (char *)malloc(add(length, 1));
    if (*result == NULL) {
        return ERROR_MEMORY_ALLOCATION;
    }

    index = 0;
    while (num > 0) {
        (*result)[index] = digits[mod(num, 1 << r)];  // get digit and convert to character
        index = add(index, 1);
        num = divide(num, 1 << r);  // divide by 2^r
    }

    (*result)[index] = '\0';

    reverseString(*result, index);

    return OK;
}

// function to reverse a string
void reverseString(char *str, int length) {
    int i;
    char tempChar;
    for (i = 0; i < (length >> 1); i = add(i, 1)) {  //  /2
        tempChar = str[i];
        str[i] = str[length - add(i, 1)];
        str[length - add(i, 1)] = tempChar;
    }
}
