/* 
 * cConfig.h ~ Header file for cConfig.c.
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
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.*
 */

#ifndef CCONFIG_H
#define CCONFIG_H

#include <stdlib.h>

struct config_opt
{
    /* Name of the option. */
    char *name;

    /* Value of the option. NULL if this opt is an array. */
    char *value;

    /* Non zero if this option is an array. */
    unsigned int is_array;

    /* Array of values. */
    char **values;

    /* Number of values. */
    size_t size;
};

typedef struct config_opt cConfig_opt;

extern void cConfig_init(void);

extern unsigned int cConfig_load(const char *);

extern cConfig_opt *cConfig_get_opt(const char *);

extern void cConfig_free(void);

extern unsigned int cConfig_find_opt_value(char *, char *);

extern char *cConfig_get_value(const char *);

extern void cConfig_print_opt(char *);

#endif
