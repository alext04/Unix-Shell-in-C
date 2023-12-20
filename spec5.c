#include "headers.h"

#define MAX_COMMANDS 15

#define PAST_EVENTS_FILE "/Users/alexthuruthel/SEM3/OSN/assignments/mini-project-1-alext04/pastevents.txt"

void add_to_pastevents(char *newString)
{
    if (strcmp(newString, "added") == 0)
    {
        return;
    }

    char *executeSubstring = strstr(newString, "pastevents execute");
    if (executeSubstring != NULL)
    {
        
        int index;
        if (sscanf(executeSubstring, "pastevents execute %d", &index) == 1)
        {
            
            FILE *file = fopen(PAST_EVENTS_FILE, "r");
            if (file == NULL)
            {
                perror("Error opening file");
                return;
            }

            char lines_1[MAX_HISTORY_SIZE + 1][MAX_COMMAND_LENGTH];
            int lineCount_1 = 0;

            
            for (int i = 0; i < MAX_HISTORY_SIZE + 1; i++)
            {
                lines_1[i][0] = '\0';
            }
           
            while (fgets(lines_1[lineCount_1], MAX_COMMAND_LENGTH, file) != NULL && lineCount_1 < MAX_HISTORY_SIZE)
            {
                lineCount_1++;
            }

            char exe_cmd[MAX_COMMAND_LENGTH];
            strcpy(exe_cmd, lines_1[index - 1]);
            strcpy(executeSubstring, exe_cmd);
            
        }
    }

    FILE *file = fopen(PAST_EVENTS_FILE, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    char lines[MAX_HISTORY_SIZE + 1][MAX_COMMAND_LENGTH];
    int lineCount = 0;

    
    for (int i = 0; i < MAX_HISTORY_SIZE + 1; i++)
    {
        lines[i][0] = '\0';
    }

    
    strcpy(lines[0], newString);
    lineCount++;

    
    while (fgets(lines[lineCount], MAX_COMMAND_LENGTH, file) != NULL && lineCount < MAX_HISTORY_SIZE)
    {
        lineCount++;
    }

    fclose(file);

    if (strcmp(lines[0], lines[1]) == 0)
    {
        return;
    }

    
    file = fopen(PAST_EVENTS_FILE, "w");
    if (file != NULL)
    {
        for (int i = 0; i < lineCount; i++)
        {
            if (lines[i][0] != '\0')
            {
                fprintf(file, "%s", lines[i]);
            }
        }
        fclose(file);
    }
}

void print_pastevents()
{
    FILE *file = fopen(PAST_EVENTS_FILE, "r"); 

    if (file == NULL)
    {
        perror("Error opening the file");
        return;
    }

    
    char **lines = NULL;
    char buffer[1024];
    size_t lineCount = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        lines = realloc(lines, (lineCount + 1) * sizeof(char *));
        if (lines == NULL)
        {
            perror("Memory allocation error");
            break;
        }

        lines[lineCount] = strdup(buffer);
        if (lines[lineCount] == NULL)
        {
            perror("Memory allocation error");
            break;
        }

        lineCount++;
    }

    
    for (size_t i = 0; i <= lineCount; i++)
    {
        printf("%s", lines[i - 1]);
        free(lines[i - 1]);
    }
    free(lines);

    
    fclose(file);
}

void purge_pastevents()
{
    FILE *file = fopen(PAST_EVENTS_FILE, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

 
    fclose(file);
}

void past_execute(int index, char *input, const char *home_dir, char *prev_dir, pidlist head)
{
    FILE *file = fopen(PAST_EVENTS_FILE, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    char lines[MAX_HISTORY_SIZE + 1][MAX_COMMAND_LENGTH];
    int lineCount = 0;

   
    for (int i = 0; i < MAX_HISTORY_SIZE + 1; i++)
    {
        lines[i][0] = '\0';
    }
    
    while (fgets(lines[lineCount], MAX_COMMAND_LENGTH, file) != NULL && lineCount < MAX_HISTORY_SIZE)
    {
        lineCount++;
    }

    char exe_cmd[MAX_COMMAND_LENGTH];
    strcpy(exe_cmd, lines[index - 1]);

    process_commands(exe_cmd, home_dir, prev_dir, head);
}