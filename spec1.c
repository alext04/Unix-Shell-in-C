
#include "headers.h"


// change this so that the executables directory works as home
void get_username_systemname_and_directory(const char *home_dir) {
    char username[256];
    char systemname[256];
    char currentdir[4096];

    struct passwd *pw = getpwuid(getuid());
    if (pw) {
        strcpy(username, pw->pw_name);
    }

    struct utsname info;
    if (uname(&info) == 0) {
        strcpy(systemname, info.nodename);
    }

    getcwd(currentdir, sizeof(currentdir));

    // Replace home directory path with ~
    if (strstr(currentdir, home_dir) == currentdir) {
        printf("<%s@%s:~%s> ", username, systemname, currentdir + strlen(home_dir));
    } else {
        printf("<%s@%s:%s> ", username, systemname, currentdir);
    }
}