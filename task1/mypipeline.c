#include "linux/limits.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

#define EXECUTION_FAILED 1
#define BUFFER_SIZE 12

int main(int argc, char **argv)
{
    char *input1[] = {"ls", "-l", 0};
    char *input2[] = {"tail", "-n", "2", 0};
    int debug = 0;
    int returnVal;
    pid_t pid1 = 0;
    pid_t pid2 = 0;
    int pipefd[2];
    int status;
    int i;

    for (i = 0; i < argc; i++)
    {
        if (strncmp(argv[i], "-d", 2) == 0)
            debug = 1;
    }

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if (debug)
    {
        fprintf(stderr, "(parent_process>forking…)\n");
        fflush(stderr);
    }
    if ((pid1 = fork()) == 0)
    {
        if (debug)
        {
            fprintf(stderr, "(child1>redirecting stdout to the write end of the pipe…)\n");
            fflush(stderr);
        }
        close(STDOUT_FILENO);
        dup(pipefd[1]);
        close(pipefd[1]);
        if (debug)
        {
            fprintf(stderr, "(child1>going to execute cmd: …)\n");
            fflush(stderr);
        }

        if ((returnVal = execvp(input1[0], input1)) < 0)
        {
            perror("couln't execute");
            _exit(EXECUTION_FAILED);
        }
    }
    else if (pid1 == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else
    {
        if (debug)
        {
            fprintf(stderr, "(parent_process>created process with id: %d)\n", pid1);
            fprintf(stderr, "(parent_process>closing the write end of the pipe…)\n");
            fflush(stderr);
        }
        close(pipefd[1]);
        if ((pid2 = fork()) == 0)
        {
            if (debug)
            {
                fprintf(stderr, "(child2>redirecting stdin to the read end of the pipe…)\n");
                fflush(stderr);
            }

            close(STDIN_FILENO);
            dup(pipefd[0]);
            close(pipefd[0]);
            if (debug)
            {
                fprintf(stderr, "(child2>going to execute cmd: …)\n");
                fflush(stderr);
            }
            if ((returnVal = execvp(input2[0], input2)) < 0)
            {
                perror("couln't execute");
                _exit(EXECUTION_FAILED);
            }
        }
        else if (pid2 == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else
        {
            if (debug)
            {
                fprintf(stderr, "(parent_process>closing the read end of the pipe…)\n");
                fflush(stderr);
            }
            close(pipefd[0]);
            if (debug)
            {
                fprintf(stderr, "(parent_process>waiting for child processes to terminate…)\n");
                fflush(stderr);
            }
            waitpid(pid1, &status, 0);
            if (debug)
            {
                fprintf(stderr, "(parent_process>waiting for child processes to terminate…)\n");
                fflush(stderr);
            }
            waitpid(pid2, &status, 0);
        }
    }

    return 0;
}
