#ifndef RANK_H
#define RANK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// function to swap elements
static inline void swap(long *a, long *b) {
  long t = *a;
  *a = *b;
  *b = t;
}

// function to find the partition position
static inline int partition(long *array, int low, int high) {
  
	// select the rightmost element as pivot
	long pivot = array[high];

	// pointer for greater element
	int i = (low - 1);

	for (int j = low; j < high; j++) {
		if (array[j] <= pivot) {
			i++;
			// swap element at i with element at j
			swap(&array[i], &array[j]);
		}
	}

	// swap the pivot element with the greater element at i
	swap(&array[i + 1], &array[high]);

	// return the partition point
	return (i + 1);
}

void quickSort(long *array, int low, int high) {
	if (low < high) {

		// find the pivot element such that
		// elements smaller than pivot are on left of pivot
		// elements greater than pivot are on right of pivot
		int pi = partition(array, low, high);

		// recursive call on the left of pivot
		quickSort(array, low, pi - 1);

		// recursive call on the right of pivot
		quickSort(array, pi + 1, high);
	}
}

static inline int rank(long *array, size_t size, int target) {
	if (size == 0 || target == 0) {
		return size;
	}
	//printf("Rank 'em, size %zd\n", size);
	long *most_likely = (long *) malloc(size * sizeof(long));
	if (most_likely == NULL) {
		// Attempt to fail gracefully
		printf("Memory allocation error a!\n");
		return size;
	}
	long *others = (long *) malloc(size * sizeof(long));
	if (others == NULL) {
		// Attempt to fail gracefully
		printf("Memory allocation error b!\n");
		return size;
	}
	//printf("Malloc'd\n");
	int likely_count = 0, others_count = 0;

	if (array == NULL) {
		// Attempt to fail gracefully
		printf("Memory allocation error!\n");
		return size;
	}

	for (uint_fast32_t i = 0; i < size; i++) {
		uint_fast32_t count = 1;
		while (i + 1 < size && array[i] == array[i + 1]) {
			count++;
			i++;
		}
		if (count == target) {
			most_likely[likely_count++] = array[i];
		}
		else {
			others[others_count++] = array[i];
		}
	}
	//printf("Likely: %d, others %d\n", likely_count, others_count);

	memcpy(array, most_likely, likely_count * sizeof(long));
	memcpy(&array[likely_count], others, others_count * sizeof(long));


	free(most_likely);
	free(others);
	return (likely_count + others_count);
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