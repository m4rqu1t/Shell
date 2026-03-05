#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "executor.h"

int executar(char *args[], int background, int style) {

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
            return -1;

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

        return -1;
    }

    if(strcmp(args[0], "cd") == 0){

        if(args[1] == NULL){

            printf("USO INCORRETO DO COMANDO cd, O CORRETO SERIA: cd <nome_da_pasta>\n");

        }else{
            if(chdir(args[1]) != 0){

                printf("ERRO NO cd\n");

            }
        }
        return -1;
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
        return -1;
    }

    int redirecionar_saida = 0;
    int modo_append;
    char *arquivo_saida = NULL;
    int k =0;

    while(args[k] != NULL){

        if(strcmp( args[k], ">>") == 0){

            redirecionar_saida = 1;
            modo_append = 1;
            arquivo_saida = args[k+1];
            args[k] = NULL;
            break;

        }else if(strcmp(args[k], ">") == 0){

            redirecionar_saida = 1;
            modo_append = 0;
            arquivo_saida = args[k+1];
            args[k] = NULL;
            break;

        }
        k++;
    }

    int redirecionar_entrada = 0;
    char *arquivo_entrada = NULL;
    int l = 0;

    while(args[l] != NULL){

        if(strcmp(args[l], "<") == 0){

            redirecionar_entrada = 1;
            arquivo_entrada = args[l+1];
            args[l] = NULL;
            break;

        }
        l++;
    }

    pid_t pid = fork();

    if(pid < 0){

        printf("ERRO NO FORK\n");
        return -1;

    }else if(pid == 0){

        if(redirecionar_saida == 1 && arquivo_saida != NULL){

            FILE *arquivo;
            if(modo_append == 1){

                arquivo = fopen(arquivo_saida, "a");

            }else{

                arquivo = fopen(arquivo_saida, "w");

            }
            if(arquivo == NULL){

                printf("ERRO AO CRIAR O ARQUIVO %s \n", arquivo_saida);
                exit(1);

            }

            int fd = fileno(arquivo);
            dup2(fd, STDOUT_FILENO);
            fclose(arquivo);

        }

        if(redirecionar_entrada == 1 && arquivo_entrada != NULL){

            FILE *arquivo_in = fopen(arquivo_entrada, "r");
            if(arquivo_in == NULL){

                printf("ERRO AO CRIAR O ARQUIVO %s \n", arquivo_entrada);
                exit(1);
            }
            int fd_in = fileno(arquivo_in);
            dup2(fd_in, STDIN_FILENO);
            fclose(arquivo_in);
        }



        if(execvp(args[0], args) == -1){

            printf("NAO EXISTE O COMANDO %s\n", args[0]);
            exit(1);
        }
    }else{

        if(background == 1){

            printf("PROCESSO NO BACKGROUND: %d\n", pid);
            return -1;

        }else if(style == 1){

            return pid;

        }else{

        int status;
        waitpid(pid, &status, 0);

        }
    }
    return -1;
}
