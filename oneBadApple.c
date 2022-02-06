#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

void sigHandler(int);
int main() {
    char userInput[10];
    char node[10];
    char message[50];

    signal (SIGINT, sigHandler);
    printf("Please enter number of nodes you want to generate:\n");
    fgets(userInput, 10, stdin);
    //numProcesses is how many nodes we need
    int numProcesses = atoi(userInput);
    //make an array of pipe with num of nodes
    //fprintf("Parent's PID is %s\n", getpid());

    //prompt user to get the message
    printf("Please input your message:\n");
    fgets(message, 50, stdin);
    //prompt user to get the node for sending the message
    printf("Which node would you like to send to?\n");
    fgets(node, 10, stdin);
    int targetNode = atoi(node);

    for(int i = 1; i < numProcesses; i++){
        pid_t pid;
        int fd[2];
        
        if(pipe(fd) != 0){
            perror("Failed to create pipe.\n");
            exit(1);
        }
        if((pid = fork()) < 0){
            perror("Failed to spawn child");
            exit(2);
        }
        //child process
        else if(pid == 0){
            printf("Child process [#]%d with [id]%d\n", i, getpid());
            //if the apple is not for them, sleep 2 seconds
            sleep(2);
            close(fd[1]);
            read(fd[0], message, sizeof(message));
            close(fd[0]);
            if(i == targetNode){
                printf("Child process [#]%d with [id]%d received the message\n", i, getpid());
            }
        }
        //parent process
        else {
            printf("Parent process [#]%d with [id] %d\n", i, getpid());
            close(fd[0]);
            write(fd[1], message, sizeof(message));
            wait(NULL);
            close(fd[1]);
        }
    }
    return 0;
}

void sigHandler (int sigNum){
    printf("Received an interrupt. Exit the program.\n");
    exit(0);
}

