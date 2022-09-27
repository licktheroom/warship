// Copyright (C) 2022  licktheroom //

//   FIXME 25/9/2022: Unable to use tables. //
//// It can only do the below:  ////
//// `output console`           ////
//// What if I wanted:          ////
//// `output file AND console`  ////
//// Maybe use {}?              ////
//// `output { file console }`  ////
//// Or &&                      ////
//// `output file && console    ////
//// Possibly this too:         ////
//// `output file||console`     ////

#include <alloca.h>
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

static bool settings_changed;

static const char * settings_file_name = "warship-settings.txt";

// Creates a default list of settings. //
bool settings_create_default()
{
    FILE * settings_file = fopen(settings_file_name, "w");

    if(!settings_file)
        return false;

    // General settings //
    fprintf(settings_file, "output console\n");

    // Finish //
    fclose(settings_file);

    return true;
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

    settings_changed = false;

    return true;
}

// Clears the settings cache and flushes it to the file. //
void settings_clear()
{
    // Make sure settings exist. //
    if(settings.first)
    {
        // Open the file for writing if any settings were changed. //
        FILE * settings_file;
        bool should_write = false;
        if(settings_changed)
        {
            settings_file = fopen(settings_file_name, "w");

            if(!settings_file)
            {
                fprintf(stdout, "Error: Failed to open \"%s\" for writting. No settings were saved.\n", settings_file_name);

                return;
            }

            should_write = true;
        }

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


            if(should_write)
                fprintf(settings_file, "%s %s\n", one->name, one->data);

            // We have to free the data, the name, and the variable itself to avoid memory leaks. //
            free(one->data);
            free(one->name);
            free(one);
        }

        if(should_write)
            fclose(settings_file);

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
                // To tell if we'll need to write to the file. //
                settings_changed = true;

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

// Creates a new var in the settings cache. //
bool settings_create_new_var(const char * name, const char * data)
{
    // Make sure settings already exist. Why do this when creating a variable? To prevent any errors when using settings_generate()
    if(settings.first)
    {
        // Allocate the memory for t_var. //
        struct t_var * new = calloc(1, (sizeof(struct t_var *) * 2) + 8);

        // Allocate the memory for name and data. //
        new->name = malloc(sizeof(char) * (strlen(name) + 1));
        new->data = malloc(sizeof(char) * (strlen(data) + 1));

        // Copy the name and data over. //
        strcpy(new->name, name);
        strcpy(new->data, data);

        // And place the new var at the end of settings. //
        settings.last->next = new;
        settings.last = new;

        settings.n_settings++;

        // Let it know we'll have to write the the file when clearing settings. //
        settings_changed = true;

        return true;
    }

    return false;
}

// Removes a var from the settings cache. //
bool settings_remove_var(const char * name)
{
    // Make sure settings exist. If settings.first and settings.last are the same don't run. This prevents an emtpy cache when it shouldn't be. //
    if(settings.first || settings.first == settings.last)
    {
        // We need three t_vars now! one is current, two is next, and three is previous. //
        struct t_var * one;
        struct t_var * two = settings.first;
        struct t_var * three = settings.first;

        unsigned int i = 0;

        while(true)
        {
            if(two == NULL || i == settings.n_settings)
                return false;

            one = two;
            two = one->next;

            if(strcmp(name, one->name) == 0)
            {
                // If one is the last var in the cache all we need to do is set settings.last to three and three.next to NULL. //
                if(one == settings.last)
                {
                    settings.last = three;
                    three->next = NULL;

                } else
                {
                    // Otherwise one is in the middle of two vars. We set three.next to two and one.next to NULL. //
                    three->next = two;

                    one->next = NULL;
                }

                // Finally free the data. //
                free(one->name);
                free(one->data);
                free(one);

                one = NULL;

                // And let settings_clear() know we'll have to write to file. //
                settings_changed = true;

                return true;
            }

            // Settings three to one at the end makes sure three is always the parent of one. //
            three = one;
        }
    }

    return false;
}
