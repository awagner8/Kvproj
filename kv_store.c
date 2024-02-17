
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "kv_store.h"

/**
 * Purpose: deserialize a key-value pair buffer (the format is in the README
 * file).
 * 
 * Inputs:
 * - `char *buf`: pointer to the key-value pair buffer.
 * - `size_t buffer_size`: total size of the key-value pair buffer (in bytes).
 * 
 * Return: `KVPAIR *start_of_list`, which is a pointer to start of linked list
 * of `KVPAIR`s if successful. Otherwise, return NULL.
 * 
 * Assumptions: `buf` is not empty or NULL and the `buffer_size` is the correct
 * size of the buffer.
 *
 * Hints: 
 * - Although you could probably make this function recursive, our
 *   implementation just uses a `while` loop.
 * - You will probably have a pointer which will go through the buffer. You
 *   first should probably think about how long you should be iterating for.
 * - Once you have figured out the correct bounds for iteration, for each new
 *   key-value pair you see, you will probably want to allocate a new node in
 *   your list, and then populate all the fields in that node. This will
 *   probably involve calling `memcpy()` a couple of times, and also `malloc()`
 *   for the value.
 * - Once you're done populating an individual node of the linked list, you are
 *   to actually add it to the list. This can be done by appending the node to
 *   the tail of the list. Think about how to do this...
 */

KVPAIR *deserialize(char *buf, size_t buffer_size) {
    KVPAIR *start = NULL;
    KVPAIR *current = NULL;
    char *ptr = buf;
    
    while (ptr < buf + buffer_size) {
        // Allocate memory for a new KVPAIR node
        KVPAIR *new_node = (KVPAIR *)malloc(sizeof(KVPAIR));
        if (new_node == NULL) {
            // Memory allocation failed
            return NULL;
        }
        
        // Copy the key from the buffer
        memcpy(&(new_node->key), ptr, sizeof(long));
        ptr += sizeof(long);
        
        // Copy the size from the buffer
        memcpy(&(new_node->size), ptr, sizeof(unsigned int));
        ptr += sizeof(unsigned int);
        
        // Allocate memory for the value string
        new_node->val = (char *)malloc(new_node->size + 1);
        if (new_node->val == NULL) {
            // Memory allocation failed
            free(new_node);
            return NULL;
        }
        
        // Copy the value string from the buffer
        memcpy(new_node->val, ptr, new_node->size);
        new_node->val[new_node->size] = '\0';
        ptr += new_node->size;
        
        // Add the new node to the linked list
        if (start == NULL) {
            start = new_node;
            current = new_node;
        } else {
            current->next = new_node;
            current = new_node;
        }
    }
    
    if (start != NULL) {
        current->next = NULL;
    }
    
    return start;
}


/**
 * Purpose: Look up the provided key in a `KVPAIR` list.
 * 
 * Inputs: 
 * - `KVPAIR *list`: pointer to the start of the linked list of `KVPAIR`s.
 * - `long key`: the key to look up in the linked list.
 * 
 * Return: `KVPAIR *found_entry`, which is pointer to the entry that contains
 * the key or NULL, if the key is not found.
 */

KVPAIR *lookup(KVPAIR *list, long key) {
    KVPAIR *current = list;
    
    while (current != NULL) {
        if (current->key == key) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}


/**
 * Purpose: Delete the `KVPAIR` with the specified key from the linked list.
 * 
 * Inputs: 
 * - `KVPAIR **list`: pointer to the pointer (handle) of first key-value pair.
 * - `long key`: the key to the `KVPAIR` entry that should be deleted.
 * 
 * Return: 1 if the key was found in the list and deleted successfully. 0 if the
 * key was not found in the list and/or some error occured during the deletion.
 * 
 * Hints:
 * - You will need to change `KVPAIR`'s next pointers in the list and free
 *   memory.
 * - Think about what needs to be updated if the first entry in the list is
 *   deleted...
 */

int delete(KVPAIR **list, long key) {
    KVPAIR *curr = *list;
    KVPAIR *prev = NULL;
    
    while (curr != NULL) {
        if (curr->key == key) {
            if (prev == NULL) {
                // The first node is to be deleted
                *list = curr->next;
            } else {
                prev->next = curr->next;
            }
            
            free(curr->val);
            free(curr);
            return 1;
        }
        
        prev = curr;
        curr = curr->next;
    }
    
    return 0;
}
