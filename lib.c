/* 
 * lib.c ~ General functions used by cConfig.
 *
 * Copyright (c) 2013 "cConfig" Niels Vanden Eynde 
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */


#include <stdio.h>
#include <string.h>

#include "lib.h"

void *
xmalloc(const size_t size)
{
    void *ptr = malloc(size);
  
    if (ptr == NULL)    
    {
        fprintf (stderr, "fatal: memory exhausted (malloc of %d bytes).\n", size);
        exit (EXIT_FAILURE);
    }

    return ptr;
}

void *
xcalloc(const size_t count, const size_t size)
{
    void *ptr;

    if (!(ptr = calloc(count, size)))
    {
        fprintf(stderr, "fatal: memory exhausted (xmalloc of %d bytes).\n", size);
        exit(EXIT_FAILURE);
    }

    return ptr;
}

void *
xrealloc(void *p, const size_t size)
{
    void *ptr = realloc(p, size);

    if (ptr == NULL)
    {
        free(p);
        fprintf (stderr, "fatal: memory exhausted (realloc of %d bytes).\n", size);
        exit (EXIT_FAILURE);
    }

    return ptr;
}

void xfree(void *ptr)
{
    if (ptr != NULL)
        free(ptr);
}


char *
dupstr(const char *string)
{
    char *dup;
    size_t len;

    len = strlen(string);
    dup = xmalloc(len + 1);
    strncpy(dup, string, len);
    dup[len] = '\0';

    return dup;
}

char **
duparr(char **array, size_t size)
{
    char **dup;
    size_t i; 

    dup = xmalloc((size + 2) * sizeof(char *));
    
    for (i = 0; i < size; ++i)
        dup[i] = dupstr(array[i]);

    dup[i + 1] = NULL;

    return dup;
}

unsigned int
explode(const char *src, const char *tokens, char ***list, size_t *len)
{   
    char *str, *copy, **_list = NULL;

    if (src == NULL || list == NULL || len == NULL)
        return 0;

    *list = NULL;
    *len  = 0;

    copy = dupstr(src);

    str = strtok(copy, tokens);

    if (str == NULL)
    {
        xfree(copy);
        return 0;
    }

    _list = xmalloc(sizeof(*_list));

    _list[*len] = dupstr(str);

    (*len)++;

    while((str = strtok(NULL, tokens)) != NULL)
    {   
        _list = xrealloc(_list, (*len + 1) * sizeof(*_list));

        _list[*len] = dupstr(str);

        (*len)++;
    }

    *list = _list;
    xfree(copy);

    return 1;
}

unsigned int
streq(const char *string1, const char *string2) 
{
    return (*string1 == *string2 && strcmp(string1, string2) == 0);
}

