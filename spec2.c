#include "headers.h"

// Function to tokenize input based on specified delimiters
void tokenize_input(char *input, char **tokens, const char *delimiters)
{
    char *token = strtok(input, delimiters);
    int i = 0;
    while (token != NULL)
    {
        tokens[i++] = token;
        token = strtok(NULL, delimiters);
    }
    tokens[i] = NULL; // Set the last element to NULL
}

// Function to process and execute commands
void process_commands(char *input, const char *home_dir, char *prev_dir, pidlist head)
{
    char commandcopy[MAX_COMMAND_LENGTH];
    strcpy(commandcopy, input);
    const char *cmd_delimiters = " \t\n"; // Space, tab, or newline
    char *commands[MAX_COMMAND_LENGTH];
    tokenize_input(input, commands, ";");

    for (int i = 0; commands[i] != NULL; i++)
    {
        char *cmd_args[MAX_COMMAND_LENGTH];
        tokenize_input(commands[i], cmd_args, cmd_delimiters);
        if (cmd_args[0] != NULL)
        {
            int background = 0;
            int arg_count = 0;

            while (cmd_args[arg_count] != NULL)
            {
                if (strcmp(cmd_args[arg_count], "&") == 0)
                {
                    cmd_args[arg_count] = NULL;
                    background = 1;
                }
                arg_count++;
            }
            // continue;
            // Check for custom commands
            if (strcmp(cmd_args[0], "warp") == 0)
            {

                if (cmd_args[1] != NULL)
                {
                    int i = 1;
                    while (cmd_args[i] != NULL)
                    {
                        warp(cmd_args[i++], home_dir, prev_dir);
                    }
                    add_to_pastevents(commandcopy);
                }
                else
                {
                    printf("Warp Arguement Error\n");
                }
            }

            else if (strcmp(cmd_args[0], "peek") == 0)
            {

                // never going into the if condition
                int show_hidden = 0;
                int show_details = 0;
                int i = 1;
                int flag_arg = 0;
                int flag_dir = 0;
                while (cmd_args[i] != NULL)
                {
                    flag_arg = 1;
                    if (strcmp(cmd_args[i], "-l") == 0)
                    {
                        show_details = 1;
                    }
                    else if (strcmp(cmd_args[i], "-a") == 0)
                    {
                        show_hidden = 1;
                    }
                    else if (strcmp(cmd_args[i], "-al") == 0 || strcmp(cmd_args[i], "-la") == 0)
                    {
                        show_hidden = 1;
                        show_details = 1;
                    }
                    else
                    {
                        flag_dir = 1;
                    }
                    i++;
                }
                if (flag_arg == 0)
                {
                    peek_directory(".", 0, 0);
                }
                else
                {

                    if (flag_dir == 1)
                    {
                        peek_directory(cmd_args[--i], show_hidden, show_details);
                    }
                    else
                    {
                        peek_directory(".", show_hidden, show_details);
                    }
                }
                add_to_pastevents(commandcopy);
            }

            else if (strcmp(cmd_args[0], "seek") == 0)
            {
                // Handle 'seek' command
                // Call your custom function for 'seek'

                int flag_f = 0;
                int flag_d = 0;
                int flag_e = 0;
                int i = 1;
                char directory[1024];
                getcwd(directory, sizeof(directory));
                char search_name[1024];
                int flag_arg = 0;
                int flag_dir = 0;
                while (cmd_args[i] != NULL)
                {
                    flag_arg = 1;
                    if (strcmp(cmd_args[i], "-f") == 0)
                    {
                        flag_f = 1;
                    }
                    else if (strcmp(cmd_args[i], "-d") == 0)
                    {
                        flag_d = 1;
                    }
                    else if (strcmp(cmd_args[i], "-e") == 0)
                    {
                        flag_e = 1;
                    }
                    else if (cmd_args[i][0] == '.' && cmd_args[i][1] != '\0')
                    {
                        // Directory argument
                        strcpy(directory, cmd_args[i]);
                    }
                    else
                    {
                        // Search name argument
                        strcpy(search_name, cmd_args[i]);
                    }
                    i++;
                }

                if (flag_d && flag_f)
                {
                    printf("INVALID FLAGS!\n");
                    return;
                }
                // printf("%s\n", search_name);
                // printf("%s\n", directory);

                seek( directory,search_name, flag_d, flag_f, flag_e);
                // seek("spec", "abc", 0, 0, 1);
                add_to_pastevents(commandcopy);
            }

            else if (strcmp(cmd_args[0], "pastevents") == 0)
            {

                if ((cmd_args[1] != NULL) && strcmp(cmd_args[1], "purge") == 0)
                {

                    purge_pastevents();
                }
                else if ((cmd_args[1] != NULL) && (strcmp(cmd_args[1], "execute") == 0))
                {

                    int count = 1;

                    printf("if condition\n");
                    if (cmd_args[2] == NULL)
                    {
                        printf("ARGUEMENT MISSING");
                        return;
                    }
                    // int index=cmd_args[2];
                    int index = atoi(cmd_args[2]);
                    past_execute(index, "filler", home_dir, prev_dir, head);
                }
                else
                {
                    print_pastevents();
                }
            }

            else if (strcmp(cmd_args[0], "proclore") == 0)
            {

                add_to_pastevents(commandcopy);
            }

            else
            {
                // Fork a new process for built-in or unrecognized command
                add_to_pastevents(commandcopy);
                pid_t pid = fork();

                if (pid == 0)
                {
                    // Child process

                    execvp(cmd_args[0], cmd_args);

                    perror("Exec failed");
                    exit(1);
                }
                else if (pid > 0)
                {
                    // Parent process

                    if (!background)
                    {

                        waitpid(pid, NULL, 0);

                        // Wait for the child process to finish
                    }
                    else
                    {
                        // Do not wait for the background process to finish
                        pidlist temp = (pidlist)malloc(sizeof(struct node));
                        temp->pid = pid;
                        strcpy(temp->command, cmd_args[0]);
                        temp->next = head->next;
                        head->next = temp;
                    }
                }
                else
                {
                    perror("Fork failed");
                }
            }
            strcpy(commandcopy, "added");
        }
    }
}
