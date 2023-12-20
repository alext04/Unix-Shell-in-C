#include "headers.h"

int main()
{
    // Keep accepting commands
    char home_dir[1024];

    if (getcwd(home_dir, sizeof(home_dir)) == NULL) {
        perror("Error getting current working directory");
        return 1;
    }
    char prev_dir[1024];
    snprintf(prev_dir, sizeof(prev_dir), "%s", home_dir);

    pidlist head =(pidlist)malloc(sizeof(struct node));
    head->next=NULL;
    while (1)
    {
        get_username_systemname_and_directory(home_dir);
        // Accept input
        char input[MAX_COMMAND_LENGTH];
        fgets(input, MAX_COMMAND_LENGTH, stdin);

        checkbg(head);

        // printf("%s\n",input);
        // Process and execute command
        // add_to_pastevents(input);
        process_commands(input,home_dir,prev_dir,head);
        
    }

    return 0;
}