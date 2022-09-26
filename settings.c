// Copyright (C) 2022  licktheroom //

// TODO 25/9/2022: A function to add a new variable to settings.                                                              //
// TODO 25/9/2022: A function to remove variables.                                                                            //
//   TODO 25/9/2022: Add a check to see if any variables have been changed, so we don't write to the file without needing to. //
//// Possibly a static bool could be used.                                                                                  ////

//   FIXME 25/9/2022: Unable to use tables. //
//// It can only do the below:  ////
//// `output console`           ////
//// What if I wanted:          ////
//// `output file AND console`  ////
//// Maybe use {}?              ////
//// `output { file console }`  ////
//// Or &&                      ////
//// `output file && console    ////

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct t_var
{
    char * name;
    char * data;

    struct t_var * next;
};

struct t_settings
{
    unsigned int n_settings;

    struct t_var * first;
    struct t_var * last;
};

static struct t_settings settings;
static const char * settings_file_name = "warship-settings.txt";

// TODO 25/9/2022: Make a default list of settings. //
bool settings_create_default()
{
    return false;
}

// Generates a list of settings. //
bool settings_generate()
{
    // Make sure settings exist. //
    if(settings.first)
        return false;

    // Try to open the settings file. //
    FILE * settings_file = fopen(settings_file_name, "r");

    // If we couldn't, try to generate a new file with default settings. //
    if(!settings_file)
        if(!settings_create_default())
            return false;

    // Make sure we are at the start of the file. //
    fseek(settings_file, 0, SEEK_SET);

    // FIXME 25/9/2022: When reading the file, the final word is read 3 times. //
    struct t_var * parent;

    // While not at the end of the file. //
    while(!feof(settings_file))
    {
        // I doubt any name or data set will be more that 255 characters long. //
        char buf[255];

        // Allocate needed memory. We need room for this t_var pointer and t_var.next. We also need room for both strings. //
        // This could be malloc instead of calloc but then valgrind complains about uninitalized variables. //
        struct t_var * current = calloc(1, (sizeof(struct t_var *) * 2) + 8);

        // Read the file to get the name. //
        fscanf(settings_file, "%s", buf);

        // Allocate the memory for it. //
        current->name = malloc(sizeof(char) * (strlen(buf) + 1));

        // And copy the name from buf into current.name //
        strcpy(current->name, buf);

        // Read the file to get the data. //
        fscanf(settings_file, "%s", buf);

        // Allocate the memory. //
        current->data = malloc(sizeof(char) * (strlen(buf) + 1));

        // And copy the data from buf into current.data //
        strcpy(current->data, buf);

        if(settings.first == NULL)
        {
            parent         = current;
            settings.first = current;
            settings.last  = current;
        } else
        {
            parent              = settings.last;
            settings.last->next = current;
            settings.last       = current;
        }

        settings.n_settings++;
    }

    // Read the fixme above.
    // This is a check to ensure we don't have an extra variable.
    if(strcmp(settings.last->name, settings.last->data) == 0)
    {
        free(settings.last->name);
        free(settings.last->data);
        free(settings.last);

        settings.last = parent;

        settings.last->next = NULL;

        settings.n_settings--;
    }

    fclose(settings_file);

    return true;
}

// TODO 25/9/2022: Make this write to the file. //

// Clears the settings cache and flushes it to the file. //
void settings_clear()
{
    // Make sure settings exist. //
    if(settings.first)
    {
        // Create two variables. //
        struct t_var * one;
        struct t_var * two = settings.first;

        unsigned int i = 0;

        // Loop through each variable in settings and free them. //
        while(true)
        {
            if(two == NULL || i == settings.n_settings)
                break;

            i++;

            // This is how we travel through the variables. Set one to two and set two to the next variable. //
            one = two;
            two = one->next;

            // We have to free the data, the name, and the variable itself to avoid memory leaks. //
            char * data = one->data;
            char * name = one->name;

            free(data);
            free(name);
            free(one);
        }

        settings.last = NULL;
        settings.first = NULL;
    }
}

// Gets a setting's value. //
char * settings_get(const char * name)
{
    // Make sure settings exist. //
    if(settings.first)
    {
        struct t_var * one;
        struct t_var * two = settings.first;

        unsigned int i = 0;

        // This is the same loop as in settings_clear() //
        while(true)
        {
            if(two == NULL || i == settings.n_settings)
                break;

            i++;

            one = two;
            two = one->next;

            // If names are the same, return the data. //
            if(strcmp(name, one->name) == 0)
                return one->data;
        }
    }

    return NULL;
}

// Sets a value. Does not update the file. //
bool settings_set(const char * name, const char * data)
{
    // Make sure settings exist. //
    if(settings.first)
    {
        struct t_var * one;
        struct t_var * two = settings.first;

        unsigned int i = 0;

        // Again, same as above. //
        while(true)
        {
            if(two == NULL || i == settings.n_settings)
                return false;

            one = two;
            two = one->next;

            // If names are the same... //
            if(strcmp(name, one->name) == 0)
            {
                // Free the already allocated memory. //
                free(one->data);

                // Then allocate the new size. //
                one->data = malloc(sizeof(char) * (strlen(data) + 1));

                // And copy it over. //
                strcpy(one->data, data);

                return true;
            }
        }
    }

    return false;
}
