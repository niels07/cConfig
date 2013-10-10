/* 
 * hash.c - Hashing.
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

#include "lib.h"
#include "hash.h"

static Hash_node *new_hash_node(const char *, void *);
static unsigned int hash(const char *);

static unsigned int
hash(const char *string)
{
    unsigned int hashval = 0;

    while (*string)
        hashval = ((hashval << 5) + hashval) + *string++;

    return hashval;
}

/* Initialize the hash table. */
Hash_table *
new_hash_table(size_t size)
{
    Hash_table *table;
    
    table = xmalloc(sizeof(Hash_table));

    table->nodes = xcalloc(size, sizeof(Hash_node *));
    table->size = size;
    table->count = 0;

    return table;
}

static Hash_node *
new_hash_node(const char *key, void *data)
{
    Hash_node *node;

    node = xmalloc(sizeof(Hash_node));

    node->key = dupstr(key);
    node->data = data;
    node->next = NULL;

    return node;
}

void 
resize_hash_table(const size_t size, Hash_table **table)
{
    Hash_table *new_table, *old_table;
    Hash_node *node, *next;
    size_t i;

    new_table = new_hash_table(size);
    old_table = *table;

    for (i = 0; i < old_table->size; ++i)
        for (node = old_table->nodes[i]; node != NULL; node = next)
        {
            next = node->next;
            insert_hash_node(node->key, node->data, new_table);
            remove_hash_node(node->key, old_table);
        }

    xfree(old_table->nodes);
    xfree(old_table);

    *table = new_table; 
}

Hash_node *
insert_hash_node(const char *key, void *data, Hash_table *table)
{
    Hash_node *node;
    unsigned int hashkey;

    hashkey = hash(key) % table->size;

    node = table->nodes[hashkey];

    while (node && node->next)
    {
        if (streq(node->key, key))
        {
            xfree(node->data);
            node->data = dupstr(data);
            return node;
        }
        node = node->next;
    }
    
    node = new_hash_node(key, data);
    node->next = table->nodes[hashkey];

    table->count++;
    table->nodes[hashkey] = node;

    return node;
}

/* Free a single node, node->data children must free
   free before calling this function. */
void
free_hash_node(Hash_node *node)
{
    xfree(node->key);
    xfree(node->data);
    xfree(node);
}

/* Remove an item from the hashtable specified by 'key'. If no item
   is found, return 0. Otherwise return 1. */
unsigned int
remove_hash_node(const char *key, Hash_table *table)
{
    Hash_node *node, *prev = NULL;
    unsigned int hashkey;

    hashkey = hash(key) % table->size;
    node = table->nodes[hashkey];

    while (node)
    {
        if (!streq(node->key, key))
        {
            prev = node;
            node = node->next;
            continue;
        }

        if (prev)
            prev->next = node->next;
        else
            table->nodes[hashkey] = node->next;

        free_hash_node(node);
        return 1;
    }
    return 0;
}

/* Free the entire hash table. */
void
free_hash_table(Hash_table *table)
{
    size_t i;
    Hash_node *node, *prev;

    for (i = 0; i < table->size; ++i)
    {
        node = table->nodes[i];
        while (node)
        {
            prev = node;
            node = node->next;
            free_hash_node(prev);
        }
    }

    xfree(table->nodes);
    xfree(table);
}

/* Find a node in the hash table identified by KEY. */
Hash_node *
find_hash_node(const char *key, Hash_table *table)
{
    unsigned int hashkey;
    Hash_node *node;

    hashkey = hash(key) % table->size;

    node = table->nodes[hashkey];

    while (node)
    {
        if (streq(node->key, key))
            return node;
        else
            node = node->next;
    }
    return NULL;
}

