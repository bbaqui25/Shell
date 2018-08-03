// Name - Bushra Baqui
// Commands to input are:
// > ls -> List information about file(s)
// > pwd -> Print Working Directory 
// > cd -> Change the current working directory
// > vim -> To create or edit a text file
// > mkdir -> Create directories
// > command > filename -> Redirects the output of command to filename. The existing contents of filename are overwritten.
// > command >> filename -> Redirects the output of command to filename. The output from command is appended to contents of filename. 
//                          Existing contents are not overwritten.
// > command < filename -> Command reads its input from filename instead of from stdin.
// > Ctrl-C -> Report that SIGINT signal has been received
// > Ctrl-Z -> report that SIGTSTP signal has been received
// Refernces used are Lab Practice code 4 & 5 
// And a youtube video by TA which demonstrates some of the commands + redirection :: https://www.youtube.com/watch?v=8WII1FbOZSs

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>

// sigint_handler() allows user to terminate a running program.
// when user enters Ctrl-C it reports that SIGINT signal has been received  by the shell by printing "SIGINT handled".
// when user enters Ctrl-Z it reports that SIGTSTP signal has been received  by the shell by printing "SIGTSTP handled".
void sigint_handler(int sig)
{
        if(sig == SIGINT)
        {
                char msg1[] = "SIGINT handled.\n361> ";
                write(1, msg1, sizeof(msg1));
        }
        if(sig == SIGTSTP)
        {
                char msg2[] = "SIGTSTP handled.\n361> ";
                write(1, msg2, sizeof(msg2));
        }
}

/************************************************************************************************************************************/

int main()
{
        // create some space for our strings
        char command[500];
        char *args[20];

        signal(SIGINT, sigint_handler);
        signal(SIGTSTP, sigint_handler);

        while(command)
        {
                printf("361> ");
                fgets(command, 500, stdin);             // read command from terminal
                command[strcspn(command, "\n")] = 0;    // strip EOL(s) char at the end:

                // repeat till user enters the command exit
                if(strcmp(command, "exit") == 0)
                {
                        exit(0);
                }

                //break the string up into words
                char *word = strtok(command, " ");
                char *filename = (char *)malloc(sizeof(char) * 50);
                int i = 0;
                int size = 0;
                int redirect;
                while (word)
                {
                        // if user sees any of these commands do redirection
                        if((strcmp(word, ">") == 0) || (strcmp(word, ">>") == 0) || (strcmp(word, "<") == 0))
                       {
                                if(strcmp(word, ">") == 0)
                                        redirect = 1;
                                if(strcmp(word, ">>") == 0)
                                        redirect = 2;
                                if(strcmp(word, "<") == 0)
                                        redirect = 3;

                                word = strtok(NULL, " ");
                                // copy a word to the filename
                                strcpy(filename, word);
                                word = strtok(NULL, " ");
                        }
                        else
                        {
                                args[i] = (char *)malloc(sizeof(char) * 100);
                                size++;
                                //copy a word to the arg array
                                strcpy(args[i], word);
                                //get next word
                                word = strtok(NULL, " ");
                                i = i + 1;
                        }

                        args[i] = (char *)malloc(sizeof(char) * 100);
                        args[i] = NULL;
                }

                //print out our array
                //int j=0;
                //for (;j<i;j++)
                //{
                //      printf("args[%d]: %s\n", j, args[j]);
                //}

                int pid = fork();
                //The parent reports the PID of the child before proceeding to the next step
                if (pid == 0)
                {
                        int line;
                        if(redirect == 1)
                        {
                                line = open(filename, O_WRONLY|O_CREAT|O_TRUNC, S_IROTH);
                                dup2(line, 1);          // command ">" is overwritten by line

                        }
                        if(redirect == 2)
                        {
                                line = open(filename, O_APPEND|O_CREAT|O_WRONLY, S_IROTH);
                                dup2(line, 2);
                        }
                        if(redirect == 3)
                        {
                                line = open(filename, O_RDONLY, S_IROTH);
                                dup2(line, 3);
                        }

                        printf("PID: %d\n", getpid());
                        printf("Exit: 0\n");
                        execvp(args[0], args);  // execvp() functions provide an array of pointers to null-terminated strings that represent the argument list available to the new program.
                        exit(0);
                }
                else
                {
                        int status;
                        wait(&status);
                }

                // free memory
                for(i = 0; i < size; i++)
                {
                        free(args[i]);
                }
        }

        return 0;
}




