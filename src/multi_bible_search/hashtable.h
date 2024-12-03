#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// How much to increase the size of the hash table by each time
#define INCREMENT_SIZE 100

// The max load of the table before reallocation
#define MAX_LOAD 0.75

// struct representing an element of the hash table
struct element
{
    char key[20];
    long* value;
    int length;
};

// struct representing the overall hash table
struct hashtable
{
    struct element** elements;
    int size;
    int num_elements;
};

// Calculate the hash of a string based on the size of the hash table
static inline int hash(const char* key, const int size) {
    size_t length = strlen(key);
    unsigned long result = key[0] << 7;
    for (size_t i = 0; i < length; i++) {
        result = (1003 * result) ^ key[i];
    }
    result += result >> 1;
    result ^= ~length;
    return result % size;
}

// Allocate space for a new or larger hash table
static inline void allocate_table(struct hashtable* ht) {
    // If the table is empty, create a brand new one
    if (ht->size == 0) {
        // We'll start with a table of INCREMENT_SIZE
        ht->elements = (struct element**) calloc(INCREMENT_SIZE, sizeof(struct element *));
        ht->num_elements = 0;
        ht->size = INCREMENT_SIZE;
        return;
    }
    else {
        int new_size = ht->size + INCREMENT_SIZE,   // The size of the new hash table
            new_hash, i, j;

        // Make a new array of elements and store the pointer to the old one
        struct element **new_elements = (struct element**) calloc(new_size, sizeof(struct element *));
        struct element **old_elements = ht->elements;

        // Copy the old hash table into the new one
        for (i = 0; i < ht->size; i++) {
            if (ht->elements[i]) {
                // Find a spot for the old element in the new table
                new_hash = hash(ht->elements[i]->key, new_size);
                j = new_hash;
                while (new_elements[j] != NULL)
                {
                    j++;
                    if (j >= new_size) {
                        j = 0;
                    }
                }
                new_elements[j] = old_elements[i];
            }
        }
        // Free the memory of the old elements and store the location of the new array
        ht->elements = new_elements;
        free(old_elements);
        ht->size = new_size;
    }
}

// Free the dynamically allocated memory for the given hash table, but not the bare table itself
void delete_table(struct hashtable* ht) {
    // If the table is empty, there's nothing to deallocate
    if (ht == NULL || !ht->size) { 
        return; 
    }

    // Otherwise, delete everything
    for (int i = 0; i < ht->size; i++) {
        // Can't free NULL
        if (ht->elements[i] != NULL) {
            free(ht->elements[i]->value);
            free(ht->elements[i]);
        }
    }
    free(ht->elements);
}

// Add an element to the hash table
void add_element(struct hashtable* ht, struct element* e) {
    // If the table is too small, expand it
    if (!ht->size) {
        allocate_table(ht);
    }
    else if (ht->size * MAX_LOAD <= ht->num_elements) {
        allocate_table(ht);
    }
    
    // Hash the element and see if we can just put it in the first spot
    int element_hash = hash(e->key, ht->size);
    if (ht->elements[element_hash] == NULL) {
        ht->elements[element_hash] = e;
        ht->num_elements++;
        return;
    }
    // Check if this element happens to be a duplicate. Probably could remove this
    if (!strncmp(ht->elements[element_hash]->key, e->key, sizeof(e->key))) {
        free(ht->elements[element_hash]->value);
        free(ht->elements[element_hash]);
        allocate_table(ht);
        element_hash = hash(e->key, ht->size);
        ht->elements[element_hash] = e;
    }
    // Since there was a collision, perform a linear search to find the next open spot
    int j = element_hash;
    while (ht->elements[j] != NULL)
    {
        j++;
        if (j == ht->size) {
            j = 0;
        }
    }
    // Add a reference to the element and increment the count of elements
    ht->elements[j] = e;
    ht->num_elements++;
}

// Get an element of the hash table
static inline struct element* get_element(const struct hashtable* ht, const char * key) {
    int j = hash(key, ht->size);
    // Search for the element. If the element does not exist, that is fine
    while (ht->elements[j] != NULL && strcmp(ht->elements[j]->key, key)) {
        j++;
        if (j == ht->size) {
            j = 0;
        }
    }
    // This returns either the element or NULL
    return ht->elements[j];
}

#endif