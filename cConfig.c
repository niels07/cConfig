/* 
 * cConfig.c
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

#include <stdio.h>
#include <stdarg.h>

#include "hash.h"
#include "lib.h"
#include "cConfig.h"

#define TABLE_SIZE 150

#define DQUOTE       '"'
#define PAREN_OPEN   '('
#define PAREN_CLOSE  ')'

#define END_LINE(c) (c == '\n' || c == '\0')

static Hash_table *config_table = NULL;

static char delim = '=';
static char comment = '#';

void 
cConfig_error(const char *format, ...)
{
    char buf[255];
    va_list vl;

    va_start(vl, format);
    vsprintf(buf, format, vl);

    fprintf(stderr, "cConfig error: %s\n", buf);
    va_end (vl);
}

void
cConfig_init(void)
{
    if (!config_table)
        config_table = new_hash_table(TABLE_SIZE);
}

cConfig_opt *
new_config_opt(const char *name, const char *value)
{
    cConfig_opt *opt;

    opt = xmalloc(sizeof(cConfig_opt));

    opt->name = dupstr(name);
    opt->value = value == NULL ? NULL : dupstr(value);

    opt->is_array = 0;
    opt->size = 1;

    return opt;
}

cConfig_opt *
cConfig_add_opt(const char *name, const char *value)
{
    cConfig_opt *opt;

    opt = new_config_opt(name, value);

    insert_hash_node(name, (void *)opt, config_table);

    return opt;
}

cConfig_opt *
cConfig_add_opt_array(const char *name, char **values, const size_t size)
{
    cConfig_opt *opt;

    opt = new_config_opt(name, NULL);
    
    opt->values = values;
    opt->is_array = 1;
    opt->size = size;

    insert_hash_node(name, (void *)opt, config_table);

    return opt;
}

cConfig_opt *
cConfig_get_opt(const char *name)
{
    cConfig_opt *opt;
    Hash_node *node;

    node = find_hash_node(name, config_table);

    if (node)
        opt = node->data;
    else
        opt = NULL;

    return opt;
}

char *
cConfig_get_value(const char *name)
{
    char *value;
    cConfig_opt *opt;

    opt = cConfig_get_opt(name);

    if (opt)
        value = opt->value;
    else
        value = NULL;

    return value;
}

void
cConfig_set_delim(char d)
{
    delim = d;
}

unsigned int
cConfig_find_opt_value(char *name, char *value)
{
    size_t i;
    cConfig_opt *opt;

    opt = cConfig_get_opt(name);

    if (opt == NULL)
        return 0;

    if (!opt->is_array)
        return 0;

    for (i = 0; i < opt->size; ++i)
    {
        if (streq(opt->values[i], value))
            return 1;
    }

    return 0;
}

void
cConfig_print_opt(char *name)
{
    char **values;
    size_t i;
    cConfig_opt *opt;

    opt = cConfig_get_opt(name);

    if (opt == NULL)
        puts("NULL ==> NULL");

    if (!opt->is_array)
    {
        fprintf(stdout, "NAME ==> %s\n", opt->name);
        fprintf(stdout, "VALUE ==> %s\n", opt->value);
        return;
    }

    values = opt->values;

    fprintf(stdout, "NAME ==> %s\n", opt->name);

    for (i = 0; i < opt->size; ++i)
        fprintf(stdout, "VALUE [%d] ==> %s\n", i, opt->values[i]);

}

static unsigned int
parse_line(char *string)
{
    char value[255], name[255], c;
    unsigned int have_name, have_quote, have_paren;
    char **values;
    size_t i = 0, len;
    
    have_name = have_quote = have_paren = 0;

    while ((c = *string++) != '\0')
    {
        if (c == DQUOTE)
        {
            if (!have_name)
            {
                cConfig_error("unexpected '%c'", DQUOTE);
                return 0;
            }
            if (have_quote && !END_LINE(*string))
            {
                cConfig_error("unexpected '%c' after '%c'", *string, DQUOTE);
                return 0;
            }
            have_quote = !have_quote;
        }
        else 
        if (c == ' ')
        {
            /* Ignore spaces outside of quotes. */
            if (have_quote)
                value[i++] = c;
        }
        else
        if (c == delim)
        {
            if (have_name)
            {
                cConfig_error("unexpected '%c'", delim);
                return 0;
            }
   
            have_name = 1;
            name[i] = '\0';
            i = 0;
        }
        else
        if (c == '\n')
            break;
        else
        if (c == PAREN_OPEN)
        {
            if (!have_name)
            {
                cConfig_error("unexpected '%c'", PAREN_OPEN);
                return 0;
            }
            have_paren = 1;
        }
        else
        if (c == PAREN_CLOSE)
        {
            if (have_paren && !END_LINE(*string))
            {
                cConfig_error("unexpected '%c' after '%c'", *string, PAREN_CLOSE);
                return 0;
            }
        }
        else
        {
            if (have_name)
                value[i++] = c;
            else
                name[i++] = c;
        }
    }

    value[i] = '\0';

    if (!have_name)
        return 1;

    if (have_paren)
    {
        explode(value, ",", &values, &len);
        cConfig_add_opt_array(name, values, len);
    }
    else
        cConfig_add_opt(name, value);

    return 1;
}

unsigned int
cConfig_load(const char *filename)
{
    FILE *f;
    char line[255];

    if ((f = fopen(filename, "r")) == NULL)
        return 0;
        
    while (fgets(line, 255, f) != NULL)
    {
        /* Ignore lines that start with a comment character. */
        if (*line == comment)
            continue;

        if (!parse_line(line))
        {
            fclose(f);
            return 0;
        }
    }
    
    fclose(f);
    return 1;
}

void
cConfig_free_opt(cConfig_opt *opt)
{
    size_t i;

    if (opt->is_array)
    {
        for (i = 0; i < opt->size; ++i)
            xfree(opt->values[i]);

        xfree(opt->values);
    }
    else
        xfree(opt->value);
    
    xfree(opt->name);
    xfree(opt);
}

void
cConfig_free(void)
{
    size_t i;
    Hash_node *node, *temp;
    cConfig_opt *opt;

    for (i = 0; i < config_table->size; ++i)
    {
        node = config_table->nodes[i];

        while (node)
        {
            opt = (cConfig_opt *)node->data;
            cConfig_free_opt(opt);

            temp = node;
            node = node->next;
            xfree(temp->key);
            xfree(temp);
        }
        config_table->nodes[i] = NULL;
    }

    xfree(config_table->nodes);
    xfree(config_table);
}

