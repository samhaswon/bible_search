#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define INCREMENT_SIZE 100
#define MAX_LOAD 0.75

struct element
{
    char key[25];
    long* value;
    int length;
};


struct hashtable
{
    struct element** elements;
    int size;
    int num_elements;
};


int hash(const char* key, int size) {
    unsigned int length = strlen(key);
    unsigned long result = key[0] << 7;
    for (int i = 0; i < length; i++) {
        result = (10000003 * result) ^ key[i];
    }
    result += result >> 1;
    result ^= ~length;
    //printf("%d\n", result % size);
    return result % size;
}


void allocate_table(struct hashtable* ht) {
    // printf("Allocation call\n");
    if (ht->size == 0) {
        // printf("Allocating initial table...\n");
        ht->elements = (struct element**) calloc(INCREMENT_SIZE, sizeof(struct element));
        ht->num_elements = 0;
        ht->size = INCREMENT_SIZE;
        // printf("Allocated table, returning\n");
        return;
    }
    else {
        // Make a new array of elements
        int new_size = ht->size + INCREMENT_SIZE,
            new_hash, i, j;
        // Make a new array of elements and store the pointer to the old one
        struct element **new_elements = (struct element**) calloc(new_size, sizeof(struct element));
        struct element **old_elements = ht->elements;
        // printf("Copying old hashtable\n");
        for (i = 0; i < ht->size; i++) {
            if (ht->elements[i]) {
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
        // printf("Deleting old hash table\n");
        ht->elements = new_elements;
        free(old_elements);
        ht->size = new_size;
        // printf("Done allocating\n");
    }
}

void delete_table(struct hashtable* ht) {
    // If the table is empty, there's nothing to deallocate
    if (!ht->size) { return; }

    // Otherwise, delete everything
    for (int i = 0; i < ht->size; i++) {
        // Can't free NULL
        if (ht->elements[i] != NULL) {
            free(ht->elements[i]->value);
            free(ht->elements[i]);
        }
    }
}


void add_element(struct hashtable* ht, struct element* e) {
    if (!ht->size) {
        allocate_table(ht);
    }
    else if (ht->size * MAX_LOAD <= ht->num_elements) {
        allocate_table(ht);
    }
    // printf("Table allocated\n");
    int element_hash = hash(e->key, ht->size);
    if (ht->elements[element_hash] == NULL) {
        ht->elements[element_hash] = e;
        ht->num_elements++;
        return;
    }
    if (!strncmp(ht->elements[element_hash]->key, e->key, sizeof(e->key))) {
        free(ht->elements[element_hash]->value);
        free(ht->elements[element_hash]);
        allocate_table(ht);
        element_hash = hash(e->key, ht->size);
        ht->elements[element_hash] = e;
    }
    int j = element_hash + 1;
    while (ht->elements[j] != NULL)
    {
        j++;
        if (j == ht->size) {
            j = 0;
        }
    }
    ht->elements[j] = e;
    ht->num_elements++;
}

struct element* get_element(struct hashtable* ht, const char * key) {
    int element_hash = hash(key, ht->size);
    int j = element_hash;
    // If the element does not exist, return NULL
    if (ht->elements[element_hash] == NULL) {
        return NULL;
    }
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