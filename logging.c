// Copyright (C) 2022  licktheroom //

#include "include/logging.h"

#include <stdio.h>
#include <string.h>

void LOGS_init()
{
    fprintf(stdout, "No log setting found, please select:\n(T)erminal, (F)ile, (B)oth: ");
    char in = (char)getchar();

    // odd bug where this prints twice when giving it something invalid, not sure why
    while(in != 't' && in != 'T' && in != 'f' && in != 'F' && in != 'b' && in != 'B')
    {
        fprintf(stdout, "\nInvalid setting, please select:\n(T)erminal, (F)ile, (B)oth: ");
        in = (char)getchar();
    }


}
