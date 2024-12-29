#include <stdio.h>
#include <stddef.h>

typedef int tvalue;

#define OK 0
#define ERROR_NULL_SEQUENCE 1
#define ERROR_NULL_SUBSEQ_START 2
#define ERROR_NULL_SUBSEQ_LENGTH 3
#define ERROR_NULL_COMPARER 4
#define ERROR_EMPTY_SEQUENCE 5

int default_comparer(tvalue const *a, tvalue const *b);
int find_the_longest_sawtooth_subsequence(tvalue const *sequence, size_t sequence_length, 
    size_t *subsequence_start_index_storage, size_t *subsequence_length_storage,
    int (*comparer)(tvalue const *, tvalue const *), int is_comparison_is_strict
);

int main() {
    tvalue sequence[] = {1, 2, 1, 1, 1, 1, 4, 2};
    size_t sequence_length = sizeof(sequence) / sizeof(sequence[0]);
    size_t subsequence_start_index = 0;
    size_t subsequence_length = 0;

    int result = find_the_longest_sawtooth_subsequence(
        sequence, sequence_length, 
        &subsequence_start_index, &subsequence_length, 
        default_comparer, 0
    );

    if (result != 0) {//prinrf("%s\n", arr[result]);
        switch (result) {
            case ERROR_NULL_SEQUENCE:
                printf("Error: sequence pointer is NULL.\n");
                break;
            case ERROR_NULL_SUBSEQ_START:
                printf("Error: subsequence start index storage pointer is NULL.\n");
                break;
            case ERROR_NULL_SUBSEQ_LENGTH:
                printf("Error: subsequence length storage pointer is NULL.\n");
                break;
            case ERROR_NULL_COMPARER:
                printf("Error: comparer function pointer is NULL.\n");
                break;
            case ERROR_EMPTY_SEQUENCE:
                printf("Error: sequence is empty.\n");
                break;
            default:
                printf("Error: unknown error code %d.\n", result);
                break;
        }
    }

    if (subsequence_length > 1) {
        printf("Longest sawtooth subsequence starts at index %zu with length %zu\n", 
            subsequence_start_index, subsequence_length);
    } else {
        printf("No sawtooth subsequence found.\n");
    }

    return 0;
}

int default_comparer(tvalue const *a, tvalue const *b) {
    return *a-*b;
}

int find_the_longest_sawtooth_subsequence(
    tvalue const *sequence, size_t sequence_length,
    size_t *subsequence_start_index_storage, size_t *subsequence_length_storage,
    int (*comparer)(tvalue const *, tvalue const *), int is_comparison_is_strict
) {
    size_t i;
    size_t current_sawtooth_length = 1;
    size_t current_sawtooth_start_index = 0;
    int expecting_greater = 1;

    *subsequence_length_storage = 1;
    *subsequence_start_index_storage = 0;

    int comparison_result = 0;
    int condition_met = 0;


    if (sequence == NULL) return ERROR_NULL_SEQUENCE;
    if (subsequence_start_index_storage == NULL) return ERROR_NULL_SUBSEQ_START;
    if (subsequence_length_storage == NULL) return ERROR_NULL_SUBSEQ_LENGTH;
    if (comparer == NULL) return ERROR_NULL_COMPARER;
    if (sequence_length == 0) return ERROR_EMPTY_SEQUENCE;
    if (sequence_length == 1) {
        *subsequence_length_storage = 1;
        *subsequence_start_index_storage = 0;
        return OK;
    }

    current_sawtooth_length = 1;
    current_sawtooth_start_index = 0;
    int expecting_greater = 1;

    *subsequence_length_storage = 1;
    *subsequence_start_index_storage = 0;

    for (i = 1; i < sequence_length; ++i) {
        int comparison_result = comparer(sequence + i - 1, sequence + i);
        int condition_met = 0;
        if (is_comparison_is_strict) {
            if (expecting_greater && comparison_result > 0) {
                condition_met = 1;
                expecting_greater = 0;
            } else if (!expecting_greater && comparison_result < 0) {
                condition_met = 1;
                expecting_greater = 1;
            }
        } else {
            if (expecting_greater && comparison_result >= 0) {
                condition_met = 1;
                expecting_greater = 0;
            } else if (!expecting_greater && comparison_result <= 0) {
                condition_met = 1;
                expecting_greater = 1;
            }
        }

        if (condition_met) {
            current_sawtooth_length++;
        } else {
            if (current_sawtooth_length > *subsequence_length_storage) {
                *subsequence_length_storage = current_sawtooth_length;
                *subsequence_start_index_storage = current_sawtooth_start_index;
            }
            current_sawtooth_start_index = i - 1;
            current_sawtooth_length = 2;
            expecting_greater = comparison_result < 0;
        }
    }

    if (current_sawtooth_length > *subsequence_length_storage) {
        *subsequence_length_storage = current_sawtooth_length;
        *subsequence_start_index_storage = current_sawtooth_start_index;
    }

    return OK;
}
