#ifndef RANK_H
#define RANK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// function to swap elements
void swap(long *a, long *b) {
  long t = *a;
  *a = *b;
  *b = t;
}

// function to find the partition position
int partition(long *array, int low, int high) {
  
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

int rank(long *array, int size, int target) {
	long *most_likely = (long *) malloc(size * sizeof(long));
	long *others = (long *) malloc(size * sizeof(long));
	int likely_count = 0, others_count = 0;

	if (target == 1) {
		for (int i = 0; i < size; i++) {
			if (i + 1 != size && array[i] != array[i + 1]) {
				most_likely[likely_count] = array[i];
				likely_count++;
			}
			else if (i + 1 == size)
			{
				most_likely[likely_count] = array[i];
				likely_count++;
			}

		}
	}
	else {
		for (int i = 0; i < size; i++) {
			for (int j = i + 1; j < size; j++) {
				if (array[i] != array[j] && j - i == target) {
					most_likely[likely_count] = array[i];
					likely_count++;
					i = j - 1;
					break;
				}
				else if (array[i] != array[j]) {
					others[others_count] = array[i];
					others_count++;
					i = j - 1;
					break;
				}
				else if (j + 1 == size && j - i + 1 == target) {
					most_likely[likely_count] = array[i];
					likely_count++;
				}
				else if (j + 1 == size && target == 1) {
					most_likely[likely_count] = array[i];
					likely_count++;
				}
				else if (j + 1 == size) {
					others[others_count] = array[i];
					others_count++;
				}
			}
		}
	}
	// printf("Likely: %d, others %d\n", likely_count, others_count);

	memcpy(array, most_likely, likely_count * sizeof(long));
	memcpy(&array[likely_count], others, others_count * sizeof(long));

	free(most_likely);
	free(others);
	return likely_count + others_count;
}

#endif