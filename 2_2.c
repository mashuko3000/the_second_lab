#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum {
    SUCCESS = 0,
    MEMORY_ALLOCATION_FAILED,
    INVALID_ARGUMENTS,
    CONCATENATION_FAILED,
    INVALID_OPERATION,
    ERROR_COUNT
} ErrorCode;

const char* error_messages[ERROR_COUNT] = {
    "Operation completed successfully.",
    "Error: Memory allocation failed.",
    "Error: Invalid argument.",
    "Error: Concatenation failed.",
    "Error: Invalid operation."
};

int str_len(const char* string);
char* reverse(const char* string);
char* second_upper(const char* string);
char* sort_string(const char* string);
int* randomization(int seed, int count);
char* concatenate(char* string, char* string_new);
void handle_error(ErrorCode code);

int main(int argc, char* argv[]) {
    if (argc < 3) {
        handle_error(INVALID_ARGUMENTS);
        return INVALID_ARGUMENTS;
    }

    int* random_int = NULL;
    int len;
    char* reversed = NULL;
    char* upper = NULL;
    char* sorted = NULL;
    char* result_string = NULL;


    switch (argv[1][1]) {
        case 'l': {
            int len = str_len(argv[2]);
            printf("Length of the string: %d\n", len);
            break;
        }
        case 'r': {
            reversed = reverse(argv[2]);
            if (reversed != NULL) {
                printf("Reversed string: %s\n", reversed);
                free(reversed);
            } else {
                handle_error(MEMORY_ALLOCATION_FAILED);
                return MEMORY_ALLOCATION_FAILED;
            }
            break;
        }
        case 'u': {
            upper = second_upper(argv[2]);
            if (upper != NULL) {
                printf("String with uppercase on odd positions: %s\n", upper);
                free(upper);
            } else {
                handle_error(MEMORY_ALLOCATION_FAILED);
                return MEMORY_ALLOCATION_FAILED;
            }
            break;
        }
        case 'n': {
            sorted = sort_string(argv[2]);
            if (sorted != NULL) {
                printf("Sorted string: %s\n", sorted);
                free(sorted);
            } else {
                handle_error(MEMORY_ALLOCATION_FAILED);
                return MEMORY_ALLOCATION_FAILED;
            }
            break;
        }
        case 'c': {
            if (argc < 4) {
                handle_error(INVALID_ARGUMENTS);
                return INVALID_ARGUMENTS;
            }

            random_int = randomization(atoi(argv[2]), argc - 2);
            if (random_int == NULL) {
                handle_error(MEMORY_ALLOCATION_FAILED);
                return MEMORY_ALLOCATION_FAILED;
            }

            result_string = argv[3]; 
            for (int i = 0; i < argc - 3; i++) {
                if (random_int[i] < argc - 2) {
                    result_string = concatenate(result_string, argv[random_int[i] + 3]);
                    if (result_string == NULL) {
                        handle_error(CONCATENATION_FAILED);
                        free(random_int);
                        return CONCATENATION_FAILED;
                    }
                }
            }
            printf("Concatenated string: %s\n", result_string);
            free(result_string);
            free(random_int);
            break;
        }
        default:
            handle_error(INVALID_OPERATION);
            return INVALID_OPERATION;
    }

    return SUCCESS;
}

int str_len(const char* string) {
    int len = 0;
    while (string[len] != '\0') {
        len++;
    }
    return len;
}

char* reverse(const char* string) {
    int len = str_len(string);
    char* reverse_str = (char*)malloc((len + 1) * sizeof(char));
    if (reverse_str == NULL) {
        return NULL;
    }

    int k = len - 1;
    for (int i = 0; i < len; i++) {
        reverse_str[i] = string[k];
        k--;
    }
    reverse_str[len] = '\0';

    return reverse_str;
}

char* second_upper(const char* string) {
    int len = str_len(string);
    char* upper_string = (char*)malloc((len + 1) * sizeof(char));
    if (upper_string == NULL) {
        return NULL;
    }

    for (int i = 0; i < len; i++) {
        if (i % 2 == 1 && islower(string[i])) {
            upper_string[i] = string[i] - 32;
        } else {
            upper_string[i] = string[i];
        }
    }
    upper_string[len] = '\0';

    return upper_string;
}

char* sort_string(const char* string) {
    int len = str_len(string);
    char* sort = (char*)malloc((len + 1) * sizeof(char));
    if (sort == NULL) {
        return NULL;
    }

    int index = 0;
    for (int i = 0; i < len; i++) {
        if (string[i] >= '0' && string[i] <= '9') {
            sort[index] = string[i];
            index++;
        }
    }

    for (int i = 0; i < len; i++) {
        if (isupper(string[i]) || islower(string[i])) {
            sort[index] = string[i];
            index++;
        }
    }

    for (int i = 0; i < len; i++) {
        if (!(string[i] >= '0' && string[i] <= '9' || isupper(string[i]) || islower(string[i]))) {
            sort[index] = string[i];
            index++;
        }
    }

    sort[len] = '\0';
    return sort;
}

int* randomization(int seed, int count) {
    int num = count - 2;
    int* arr = (int*)malloc(num * sizeof(int));
    if (arr == NULL) {
        return NULL;
    }

    for (int i = 0; i < num; i++) {
        arr[i] = i + 1;
    }

    for (int i = num - 1; i > 0; i--) {
        int j = (seed + i) % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    return arr;
}

char* concatenate(char* string, char* string_new) {
    int len1 = str_len(string);
    int len2 = str_len(string_new);

    char* temp_string = (char*)realloc(string, (len1 + len2 + 1) * sizeof(char));
    if (temp_string == NULL) {
        free(string);
        free(string_new);
        return NULL;
    }
    string = temp_string;

    int index = 0;
    for (int i = len1; i < len1 + len2; i++) {
        string[i] = string_new[index++];
    }

    string[len1 + len2] = '\0';
    return string;
}

void handle_error(ErrorCode error) {
    if (error >= 0 && error < ERROR_COUNT) {
        printf("%s\n", error_messages[error]);
    } else {
        printf("Invalid error code.\n");
    }
}
