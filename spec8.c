
#include "headers.h"

// -e flag is left
void seek(const char *current_path, const char *target_name, int d_flag, int f_flag, int e_flag)
{
    DIR *dir = opendir(current_path);
    if (dir == NULL)
    {
        perror("Error opening directory");
        return;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || entry->d_name[0] == '.')
        {
            continue;
        }

        char sub_path[1024];
        snprintf(sub_path, sizeof(sub_path), "%s/%s", current_path, entry->d_name);

        struct stat stat_buf;
        if (lstat(sub_path, &stat_buf) == -1)
        {
            perror("Error getting file/directory info");
            continue;
        }

        char *filename = strrchr(sub_path, '/');
        if (filename != NULL)
        {
            filename++; 
        }
        else
        {
            filename = sub_path; 
        }

        if (S_ISDIR(stat_buf.st_mode))
        {
            if ((!d_flag && !f_flag) && strstr(filename, target_name))
            {
                printf("%s\n", sub_path); 
            }
            if ((d_flag && !f_flag) && strstr(filename, target_name))
            {
                printf("%s\n", sub_path);
            }
            
            seek(sub_path, target_name, d_flag, f_flag, e_flag); 
        }
        else if (S_ISREG(stat_buf.st_mode))
        {
            if ((!d_flag && !f_flag) && strstr(filename, target_name))
            {
                printf("%s\n", sub_path); 
            }

            if ((!d_flag && f_flag) && strstr(filename, target_name))
            {
                printf("%s\n", sub_path); 
            }
        }
    }

    closedir(dir);
}