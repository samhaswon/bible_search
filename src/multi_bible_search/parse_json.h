#ifndef PARSE_JSON_H
#define PARSE_JSON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hashtable.h"

#define TOKEN_MAX_LENGTH 24

// Count the number of a character in a string up to length
static inline size_t char_count(const char *str, const char character, int len) {
    size_t count = 0;
    for (int i = 0; i < len; i++) {
        if (str[i] == character) {
            count++;
            // Each reference is at least 7 characters long, so jump by that much
            i += 7;
        }
    }
    return count;
}

// Function to parse the JSON
static inline void parse_json(const char *json, struct hashtable *ht) {
    // Parse the JSON data
    char *ptr = json;
    while ((ptr = strchr(ptr, '\"')) != NULL) {
        char *token_start = ptr + 1;
        char *token_end = strchr(token_start, '\"');
        if (!token_end) {
            break;
        }

        size_t token_length = token_end - token_start;
        if (token_length > TOKEN_MAX_LENGTH) {
            break;
        }

        struct element *e = (struct element *) malloc(sizeof(struct element));
        strncpy(e->key, token_start, token_length);
        e->key[token_length] = '\0';

        char *array_start = strchr(token_end + sizeof(char), '[');
        char *array_end = strchr(array_start, ']');

        array_start++;
        char *num_start = array_start, 
             *num_end;
        size_t array_size = 0;
        long *values = NULL, value;

        values = (long *) malloc((char_count(array_start, ',', array_end - array_start) + 1) * sizeof(long));

        while (num_start < array_end) {
            value = strtol(num_start, &num_end, 10);
            values[array_size++] = value;

            num_start = num_end + 1;
        }

        e->value = values;
        e->length = array_size;

        add_element(ht, e);

        ptr = array_end + 1;
    }

}


#endif