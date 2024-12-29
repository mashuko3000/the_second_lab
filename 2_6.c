#include <stdio.h>
#include <stdarg.h>

#define OK 0
#define ERROR_INVALID_ARGUMENTS -1
#define ERROR_NULL_POINTER -2

int evaluate_polynomial(double x, int degree, double *result, ...);

int main() {
    double x = 3.0;
    int degree = 2;
    double result = 0.0;
    int status;

    status = evaluate_polynomial(x, degree, &result, 1.0, -7.0, 10.0);
    
    if (status != OK) {
        if (status == ERROR_INVALID_ARGUMENTS) {
            printf("error: invalid arguments\n");
        } else if (status == ERROR_NULL_POINTER) { 
            printf("error: null pointer encountered\n");
        } else { 
            printf("unknown error: %d\n", status);
        }
        return status;
    } else {
        printf("The value of the polynomial at x = %.2f is: %.2f\n", x, result);
    }

    return OK;
}

int evaluate_polynomial(double x, int degree, double *result, ...) {
    int i;
    double coeff;
    va_list args;
    
    if (degree < 0) {
        return ERROR_INVALID_ARGUMENTS;
    }
    if (result == NULL) {
        return ERROR_NULL_POINTER;
    }

    va_start(args, result);

    *result = 0.0;

    for (i = 0; i <= degree; i++) {
        coeff = va_arg(args, double);
        *result = *result * x + coeff;
    }

    va_end(args);
    return OK;
}
