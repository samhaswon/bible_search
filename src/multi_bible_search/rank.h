#ifndef RANK_H
#define RANK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
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
static inline void merge(long* dest, size_t dest_len, long* src, size_t src_len) {
    // Copy of the old destination array
    long *old_dest = (long *) malloc(dest_len * sizeof(long));
    if (!old_dest) {
        printf("Memory allocation failure\n");
        return;
    }
	memcpy_long(old_dest, dest, dest_len);

    size_t i = 0,	// Iterator for `old_dest` array
	       j = 0, 	// Iterator for `src` array
		   k = 0;	// Iterator for `dest` array

	// Merge the two arrays low to high
    while (i < dest_len && j < src_len) {
        if (old_dest[i] < src[j]) {
            dest[k] = old_dest[i];
            i++;
        }
        else {
            dest[k] = src[j];
            j++;
        }
        k++;
    }

	// Copy what may remain in either array after merging
    if (i < dest_len) {
        memcpy_long(&dest[k], &old_dest[i], (dest_len - i));
    }
    else if (j < src_len) {
        memcpy_long(&dest[k], &src[j], (src_len - j));
    }

    // Free the old destination array
    free(old_dest);
}

// Rank elements in the result `array` by their frequency
static inline int rank(long *array, size_t size, int target, Py_ssize_t max_results) {
	if (size == 0 || target == 0) {
		return size;
	}
	// Array of the most likely search results
	long *most_likely = (long *) malloc(size * sizeof(long));
	if (most_likely == NULL) {
		// Attempt to fail gracefully
		printf("Memory allocation error a!\n");
		return size;
	}
	// Array of the other results
	result_pair *others = (result_pair *) malloc(size * sizeof(result_pair));
	// Temporary result_pair for creating the array of other results
	result_pair tmp_other;
	// Max for counting sort
	uint_fast16_t max = 0;
	if (others == NULL) {
		// Attempt to fail gracefully
		printf("Memory allocation error b!\n");
		return size;
	}
	
	int likely_count = 0, 	// Size of the likely array
		others_count = 0;	// Size of the others array

	// Find duplicates of the desired count
	for (uint_fast32_t i = 0; i < size; i++) {
		uint_fast32_t count = 1;
		// Find the count of this element
		while (i + 1 < size && array[i] == array[i + 1]) {
			count++;
			i++;
		}
		// If it's what we're looking for, add it to the likely array
		if (count == target) {
			most_likely[likely_count++] = array[i];
		}
		// Otherwise, put it in others
		else {
			tmp_other.element = array[i];
			tmp_other.count = count;
			others[others_count++] = tmp_other;
			if (count > max) {
				max = count;
			}
		}
	}

	// Copy the temporary arrays into the old one
	memcpy_long(array, most_likely, likely_count);

	if (likely_count < max_results) {
		// Sort the other results and add them to the correct place in the array
		countingSort(others, others_count, &array[likely_count], max);

		// Free dynamically allocated memory
		free(most_likely);
		free(others);
		return (likely_count + others_count);
	}

	// Free dynamically allocated memory
	free(most_likely);
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