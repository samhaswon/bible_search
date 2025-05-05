#ifndef RANK_H
#define RANK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "memcpy_long.h"

typedef struct result_pair {
    long element;
    uint_fast16_t count; 
} result_pair;

// Counting sort function for result_pair array
static inline void countingSort(result_pair arr[], int n, long* destination, const uint_fast16_t max) {
    // Create a count array to store count of each unique count value
    int *count = (int *)calloc(max + 1, sizeof(int));

    // Store the count of each count value
    for (int i = 0; i < n; i++) {
        count[arr[i].count]++;
    }

    // Change count[i] so that count[i] contains the actual position of this count in the output array
    for (int i = max - 1; i >= 0; i--) {
        count[i] += count[i + 1];
    }

    // Build the output array
    int tmp_count;
    for (int i = n - 1; i >= 0; i--) {
        tmp_count = arr[i].count;
        destination[count[tmp_count] - 1] = arr[i].element;
        count[tmp_count]--;
    }

    // Clean up
    free(count);
}

/*
 * Merge two (individually sorted) lists together.
 * Assumes that the size of `dest` is `dest_len + src_len` 
 */
static inline size_t merge_results(result_pair* dest, size_t dest_len, const long* src, size_t src_len) {
    // Copy of the old destination array
    result_pair *old = malloc(dest_len * sizeof(result_pair));
    if (!old) {
        printf("Memory allocation failure in merge_results\n");
        return 0;
    }
    memcpy(old, dest, dest_len * sizeof(result_pair));

    size_t i = 0, j = 0, k = 0;
    
    // Merge old (with counts) and src into dest
    while (i < dest_len && j < src_len) {
        long val_old = old[i].element;
        long val_src = src[j];
         
        if (val_old == val_src) {
            // Copy old entry, then add src occurrences
            dest[k] = old[i];
            // Count how many times src[j] repeats
            uint_fast16_t extra = 0;
            while (j < src_len && src[j] == val_src) {
                extra++;
                j++;
            }
            dest[k].count += extra;
            k++;
            i++;
        } 
        else if (val_old < val_src) {
            dest[k++] = old[i++];
        }
        else {
            // New element from src
            long current = val_src;
            uint_fast16_t cnt = 0;
            while (j < src_len && src[j] == current) {
                cnt++;
                j++;
            }
            dest[k].element = current;
            dest[k].count = cnt;
            k++;
        }
    }
    
    // Copy remaining old entries
    while (i < dest_len) {
        dest[k++] = old[i++];
    }
    
    // Copy remaining src entries
    while (j < src_len) {
        long current = src[j];
        uint_fast16_t cnt = 0;
        while (j < src_len && src[j] == current) {
            cnt++;
            j++;
        }
        dest[k].element = current;
        dest[k].count = cnt;
        k++;
    }

    free(old);
    return k;
}

// Rank elements in the result `array` by their frequency
static inline size_t rank(result_pair *array, size_t size, int target, Py_ssize_t max_results, long* token_target) {
    if (size == 0 || target == 0) {
        return 0;
    }
    // Array of the other results
    result_pair *others = (result_pair *) malloc(size * sizeof(result_pair));
    // Max for counting sort
    uint_fast16_t max = 0;
    if (others == NULL) {
        // Attempt to fail gracefully
        printf("Memory allocation error b!\n");
        return 0;
    }
    
    int likely_count = 0,        // Size of the likely array
        others_count = 0;        // Size of the others array
    size_t max_index = size - 1; // The maximum index of the array

	// The current target element
	long element;

    // Find duplicates of the desired count
    for (uint_fast32_t i = 0; i < size; i++) {
        // If it's what we're looking for, add it to the likely array
        if (array[i].count == target) {
            token_target[likely_count++] = array[i].element;
        }
        // Otherwise, put it in others
        else {
            others[others_count++] = array[i];
            if (array[i].count > max) {
                max = array[i].count;
            }
        }
    }

    if (likely_count < max_results) {
        // Sort the other results and add them to the correct place in the array
        countingSort(others, others_count, &token_target[likely_count], max);

        // Free dynamically allocated memory
        free(others);
        return (likely_count + others_count);
    }

    // Free dynamically allocated memory
    free(others);
    return max_results;
}

// Convert a string to lowercase
static inline void make_lower(char *string) {
    for ( ; *string; ++string) {
        // Verbose, but this is fast
        switch (*string)
        {
        case 'A':
            *string = 'a';
            break;

        case 'B':
            *string = 'b';
            break;

        case 'C':
            *string = 'c';
            break;

        case 'D':
            *string = 'd';
            break;
        
        case 'E':
            *string = 'e';
            break;

        case 'F':
            *string = 'f';
            break;

        case 'G':
            *string = 'g';
            break;

        case 'H':
            *string = 'h';
            break;

        case 'I':
            *string = 'i';
            break;

        case 'J':
            *string = 'j';
            break;

        case 'K':
            *string = 'k';
            break;

        case 'L':
            *string = 'l';
            break;

        case 'M':
            *string = 'm';
            break;

        case 'N':
            *string = 'n';
            break;

        case 'O':
            *string = 'o';
            break;

        case 'P':
            *string = 'p';
            break;

        case 'Q':
            *string = 'q';
            break;

        case 'R':
            *string = 'r';
            break;

        case 'S':
            *string = 's';
            break;

        case 'T':
            *string = 't';
            break;

        case 'U':
            *string = 'u';
            break;

        case 'V':
            *string = 'v';
            break;

        case 'W':
            *string = 'w';
            break;

        case 'X':
            *string = 'x';
            break;

        case 'Y':
            *string = 'y';
            break;

        case 'Z':
            *string = 'z';
            break;
        
        default:
            break;
        }
    }
}

#endif