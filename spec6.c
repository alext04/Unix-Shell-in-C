#include "headers.h"

void checkbg(pidlist head)
{
    pidlist temp = (pidlist)malloc(sizeof(struct node));
    temp = head->next;
    pidlist prev=head;
    while (temp != NULL)
    {
        int status;
        pid_t background_pid=temp->pid;
        pid_t result = waitpid(background_pid, &status, WNOHANG);

        if (result == background_pid)
        {
            if (WIFEXITED(status) || WIFSIGNALED(status))
            {
                printf("%s exited normaly (%d)\n", temp->command,background_pid);
                
            }
            prev->next=temp->next;

        }
        temp=temp->next;
    }
}