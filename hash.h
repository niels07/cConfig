/* 
 * hash.h - Header file for hash.c.
 *
 * Copyright (c) 2012 "cConfig" Niels Vanden Eynde 
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef CCONFIG_HASH_H
#define CCONFIG_HASH_H

#include <string.h>
#include "lib.h"

struct hash_node
{
    /* Key to find a node. */
    char *key;

    /* The data we need. */
    void *data;

    /* Next hash in the table. */
    struct hash_node *next;
};

typedef struct hash_node Hash_node;

struct hash_table
{
    /* Number of allocated nodes. */
    size_t size;

    /* Number of nodes in the hash table. */
    size_t count;

    /* The hash nodes in the hash table. */
    Hash_node **nodes;
};

typedef struct hash_table Hash_table;

extern Hash_table *new_hash_table(size_t);
extern Hash_node *insert_hash_node(const char *, void *, Hash_table *);
extern void free_hash_node(Hash_node *);
extern unsigned int remove_hash_node(const char *, Hash_table *);
extern void free_hash_table(Hash_table *);
extern Hash_node *find_hash_node(const char *, Hash_table *);
extern void resize_hash_table(const size_t, Hash_table **);

#endif /* CCCONFIG_HASH_H */
