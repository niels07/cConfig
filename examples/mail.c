#include <cConfig.h>

#include <string.h>
#include <stdio.h>

char *
read(const char *str)
{
    char line[50];
    size_t len;

    fprintf(stdout, "%s ", str);
    fgets(line, 50, stdin);

    len = strlen(line) - 1;

    if (line[len])
        line[len] = '\0';

    return strdup(line);
}

void
send_mail(char *mail, char *subject, char *message)
{
    /* Send an email. */
    printf("Your email with message '%s' titled '%s' has been send to '%s'.\n", message, subject, mail);
}

int main(void)
{
    char *to, *subject, *message, *mail;

    cConfig_init();
    cConfig_load("mail.conf");

    to = read("To:");    
    subject = read("Subject:");    
    message = read("Message:");

    if (!cConfig_find_opt_value("users", to))
    {
        fprintf(stderr, "Error: User (%s) not found in users list.\n", to);
        return 1;
    }

    mail = cConfig_get_value(to);

    send_mail(mail, subject, message);

    free(subject);
    free(message);
    free(to);

    cConfig_free();
    
    return 0;
}
