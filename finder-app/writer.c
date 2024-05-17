#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

int main(int argc, char *argv[])
{
    openlog("writer", 0, LOG_USER);

    if (argc < 3)
    {
        printf("Invalid Number of arguments.\n");
        syslog(LOG_ERR, "Invalid Number of arguments: %d", argc);
        exit(1);
    }

    char * writefile = argv[1];
    char * writestr = argv[2];

    FILE *file = fopen(writefile,"w");

    if (file == NULL)
    {
        printf("Cannot create file %s.\n", writefile);
        syslog(LOG_ERR, "Cannot create file");
        exit(1);
    }

    fprintf(file, "%s", writestr);
    syslog(LOG_DEBUG, "Writing %s to %s", writestr, writefile);

    fclose(file);

    return 0;
}
