// Copyright (C) 2022  licktheroom //

#include "include/logging.h"

#include <stdio.h>
#include <string.h>

FILE * LOGS_get_log_file(int n)
{
    char in = '\0';

    if(n == 0)
    {
        // to be updated to actually read options
        fprintf(stdout, "No log setting found, please select:\n(T)erminal, (F)ile: ");
        in = (char)getchar();

        // odd bug where this prints twice when giving it something invalid, not sure why
        while(in != 't' && in != 'T' && in != 'f' && in != 'F')
        {
            fprintf(stdout, "\nInvalid setting, please select:\n(T)erminal, (F)ile: ");
            in = (char)getchar();
        }

        // check for save
        fprintf(stdout, "Would you like to save this setting? (Y)es\\(N)o: ");

        char save = (char)getchar();

        // this does the same bug as above
        while(save != 'y' && save != 'Y' && save != 'n' && save != 'N')
        {
            fprintf(stdout, "\nInvalid setting, please select (Y)es\\(N)o: ");

            save = (char)getchar();
        }

        if(save == 'y' || save == 'Y')
            fprintf(stdout, "not saved anyway\n");

    } else
    {
        if(n == 0)
            in = 't';
        else
            in = 'f';
    }

    if(in == 'F' || in == 'f')
    {
        FILE * fe = fopen("logs.txt", "w");
        fseek(fe, 0, SEEK_SET);

        return fe;

    } else
        return stdout;
}
