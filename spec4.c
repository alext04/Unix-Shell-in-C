#include "headers.h"






void print_permissions(mode_t mode) {
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

void peek_directory(const char *path, int show_hidden, int show_details) {
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror("peek");
        return;
    }

    struct dirent *entry;
    // make this lexographic order

    
    while ((entry = readdir(dir)) != NULL) {
        // Skip hidden files if not requested
        if (!show_hidden && entry->d_name[0] == '.') {
            continue;
        }

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat file_stat;
        if (stat(full_path, &file_stat) != 0) {
            perror("stat");
            continue;
        }

        // Formatting and color coding
        if (S_ISDIR(file_stat.st_mode)) { // Directory
            printf("\033[1;34m");
        } else if (file_stat.st_mode & S_IXUSR) { // Executable
            printf("\033[1;32m");
        }
        
        if (show_details) {
            print_permissions(file_stat.st_mode);
            printf(" %2hu ", file_stat.st_nlink);

            struct passwd *pw = getpwuid(file_stat.st_uid);
            if (pw != NULL) {
                printf("%s ", pw->pw_name);
            } else {
                printf("%d ", file_stat.st_uid);
            }

            struct group *gr = getgrgid(file_stat.st_gid);
            if (gr != NULL) {
                printf("%s ", gr->gr_name);
            } else {
                printf("%d ", file_stat.st_gid);
            }

            printf("%8lld ", file_stat.st_size);

            char time_buffer[80];
            strftime(time_buffer, sizeof(time_buffer), "%b %d %H:%M", localtime(&file_stat.st_mtime));
            printf("%s ", time_buffer);
        }
        
        printf("%s", entry->d_name); // Print file/directory name

        // Reset color
        if (S_ISDIR(file_stat.st_mode) || (file_stat.st_mode & S_IXUSR)) {
            printf("\033[0m");
        }
        printf("\n");
    }

    closedir(dir);
}