#include <cConfig.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    cConfig_opt *opt;
    int i;

    if (argc != 2)
        return 1;

    cConfig_init();
    cConfig_load("simple.conf");

    opt = cConfig_get_opt(argv[1]);

    if (opt == NULL)
    {
        puts("not found");
        return 1;
    }

    if (opt->is_array)
    {
        for (i = 0; i < opt->size; ++i)
            printf("%d. %s\n", i + 1, opt->values[i]);

    }
    else
        printf("%s\n", opt->value);

    cConfig_free();

    return 0;
}
