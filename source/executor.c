#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "executor.h"

void executar(char *args[], int background) {
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
