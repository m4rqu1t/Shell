#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "executor.h"

void executar(char *args[], int background) {

    int i = 0;
    int temPipe = 0;
    int posPipe = 0;

    while(args[i] != NULL){
        if(strcmp(args[i], "|") == 0){
            temPipe = 1;
            posPipe = i;
            break;
        }
        i++;
    }

    if(temPipe == 1){
        args[posPipe] = NULL;
        char **args2 = &args[posPipe +1];

        int fd[2];
        if(pipe(fd) == -1){
            printf("ERRO NO PIPE\n");
            return;
        }

        pid_t pid1 = fork();
        if(pid1 == 0){

            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);

            execvp(args[0], args);
            printf("ERRO NO COMANDO DO PIPE\n");
            exit(1);
        }

        pid_t pid2 = fork();
        if(pid2 == 0){

            dup2(fd[0], STDIN_FILENO);
            close(fd[1]);
            close(fd[0]);

            execvp(args2[0], args2);
            printf("ERRO NO SEGUNDO COMANDO DO PIPE\n");
            exit(1);
        }

        close(fd[0]);
        close(fd[1]);

        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);

        return;
    }

    if(strcmp(args[0], "cd") == 0){
        if(args[1] == NULL){
            printf("USO INCORRETO DO COMANDO cd, O CORRETO SERIA: cd <nome_da_pasta>\n");
        }else{
            if(chdir(args[1]) != 0){
                printf("ERRO NO cd\n");
            }
        }
        return;
    }

    if(strcmp(args[0], "fg") == 0){
        if(args[1] == NULL){

            printf("USO INCORRETOD O COMANDO fg, O CORRETOR SERIA: fg <numero_do_pid>\n");

        }else{
                pid_t pid_alvo = atoi(args[1]);
                printf("PEGANDO O PROCESSO %d PARA O FG\n", pid_alvo);
                int status;
                waitpid(pid_alvo, &status, 0);
        }
        return;
    }







    pid_t pid = fork();

    if(pid < 0){

        printf("ERRO NO FORK\n");
        exit(0);
    }else if(pid == 0){

        if(execvp(args[0], args) == -1){

            printf("NAO EXISTE O COMANDO %s\n", args[0]);
            exit(1);
        }
    }else{

        if(background == 1){
            printf("PROCESSO NO BACKGROUND: %d\n", pid);
        }else{

        int status;
        waitpid(pid, &status, 0);
        }
    }
}
