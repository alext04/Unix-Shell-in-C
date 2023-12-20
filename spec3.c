
#include "headers.h"


void warp(char *path, const char *home_dir, char *prev_dir)
{
    char full_path[1024]; // Assuming a maximum path length of 1024 characters
    if (path[0] == '~' || path[0] == '-' || path[0] == '.' || path[1] == '.')
    {

        if (path[0] == '~')
        {
            // Get home directory
            snprintf(full_path, sizeof(full_path), "%s%s", home_dir, path + 1);
        }
        else if (path[0] == '-')
        {

            if (prev_dir)
            {

                snprintf(full_path, sizeof(full_path), "%s", prev_dir);
            }
            else
            {
                fprintf(stderr, "warp: OLDPWD not set\n");
                return;
            }
        }
        else if (path[1] == '.')
        {
            if (chdir("..") == 0)
            {
                getcwd(full_path, sizeof(full_path));
            }
            else
            {
                perror("warp: Error navigating to parent directory");
                return;
            }
        }
        else if (path[0] == '.')
        {

            getcwd(full_path, sizeof(full_path));
        }
    }
    else
    {
        // Handle relative or absolute paths
        if (path[0] == '/')
        {
            // Absolute path
            snprintf(full_path, sizeof(full_path), "%s", path);
        }
        else
        {
            // Relative path
            getcwd(full_path, sizeof(full_path));
            snprintf(full_path, sizeof(full_path), "%s/%s", full_path, path);
        }
    }

    // Change the directory
    if (prev_dir)
    {
        snprintf(prev_dir, sizeof(full_path), "%s", getcwd(NULL, 0));
    }
    if (chdir(full_path) == 0)
    {
    }
    else
    {
        perror("warp");
    }
}