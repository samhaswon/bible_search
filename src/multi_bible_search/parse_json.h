#ifndef PARSE_JSON_H
#define PARSE_JSON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hashtable.h"

// Tell MSVC it's fine
#pragma warning(disable : 4996)

// Maximum length of the string of a token (the buffer size - 1)
#define TOKEN_MAX_LENGTH 24
#define REF_MIN_LEN 4
#define REF_NUM_BASE 36

// Count the number of a character in a string up to length
static inline size_t char_count(const char *str, const char character, int len) {
    size_t count = 0;
    for (int i = 0; i < len; i++) {
        if (str[i] == character) {
            count++;
            // Each reference is at least 4 characters long, so jump by that much.
            i += REF_MIN_LEN;
        }
    }
    return count;
}

// Function to parse the JSON
static inline void parse_json(const char *json, struct hashtable *ht) {
    // Parse the JSON data
    char *ptr = json;
    while ((ptr = strchr(ptr, '\"')) != NULL) {
        // Find the token
        char *token_start = ptr + 1;
        char *token_end = strchr(token_start, '\"');

        // Length of the search token
        size_t token_length = token_end - token_start;
        if (token_length > TOKEN_MAX_LENGTH) {
            break;
        }

        // Temporary element to be added to the hashtable
        struct element *e = (struct element *) malloc(sizeof(struct element));
        // Copy the key to the element and null terminate it
        strncpy(e->key, token_start, token_length);
        e->key[token_length] = '\0';

        // Start of the array of references
        char *array_start = strchr(token_end + sizeof(char), '[');
        // Each reference is at least 7 characters long, so jump by that much
        // End of the array of references
        char *array_end = strchr(array_start + sizeof(char) * REF_MIN_LEN, ']');

        // Skip the starting bracket itself
        array_start++;
        char *num_start = array_start,  // Starting index of the string for the number
             *num_end;                  // Ending index of the string for the number
        size_t array_size = 0;          // Size/index of the array of references
        long *values = NULL,            // Array of values from the JSON data
              value;                    // Parsed value

        // Allocate the array of values based on the number of ',' separators + 1
        values = (long *) malloc((char_count(array_start, ',', array_end - array_start) + 1) * sizeof(long));

        while (num_start < array_end) {
            // Convert the string to a long and add it to the array
            value = strtol(num_start, &num_end, REF_NUM_BASE);
            values[array_size++] = value;

            // since num_end is the index in the string just after the number (a ','), add 1 to it and use that as our start
            num_start = num_end + 1;
        }

        // Add the array to the element
        e->value = values;
        e->length = array_size;

        // Add the element to the hash table
        add_element(ht, e);

        // Move ptr to the ',' at the end of the array. 
        ptr = array_end + 1;
    }

}


#endif