#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "executor.h"

int main(int argc, char **argv) {
    char input[1001];
    char *args[1001];
    int background;
    int style = 0;

    FILE *entrada = stdin;
    int lendo_arq = 0;

    if(argc >2){

        printf("SO PODE PASSAR UM ARQUIVO POR VEZ!\n");
        return 1;

    }

    if(argc > 1){

        entrada = fopen(argv[1], "r");

        if(entrada == NULL){

            printf("ERRO AO ABRIR O ARQUIVO %s\n", argv[1]);
            return 1;

        }

        lendo_arq = 1;

    }

    while(1) {

        if(lendo_arq == 0){
            if(style == 0){

                printf("Shell BRABO seq >");

            }else{

            printf("Shell BRABO par >");

            }
        }

        if (fgets(input, 1001, entrada) == NULL) {

            printf("\nFECHANDO\n");
            break;

        }

        int i = 0;

        while (input[i] != '\n' && input[i] != '\0'){

            i++;

        }

        if (input[i] == '\n'){

            input[i] = '\0';

        }

        char *comandos[1001];
        int num_cmds = 0;
        char *p = input;

        while(*p != '\0'){

            while(*p == ';' || *p == ' '){

                p++;

            }

            if(*p == '\0'){

                break;

            }

            comandos[num_cmds] = p;
            num_cmds++;

            while(*p != ';' && *p != '\0'){

                p++;

            }

            if(*p == ';'){

                *p = '\0';
                p++;

            }
        }

        int pids[1001];
        int num_pids = 0;

        for (int c = 0; c < num_cmds; c++) {

            if (ler_e_cortar(comandos[c], args, &background)) {

                if (strcmp(args[0], "style") == 0) {

                    if(args[1] != NULL && strcmp(args[1], "parallel") == 0){

                        style = 1;

                    }else if(args[1] != NULL && strcmp(args[1], "sequential") == 0){

                        style = 0;

                    }else{

                        printf("USO CORRETO: style <parallel | sequential>\n");

                    }
                    continue;
                }

                int pid_gerado = executar(args, background, style);

                if(style == 1 && background == 0 && pid_gerado > 0){
                    pids[num_pids] = pid_gerado;
                    num_pids++;
                }
            }
        }

        if(style == 1){

            for(int p = 0; p < num_pids; p++){

                waitpid(pids[p], NULL, 0);

            }
        }
    }

    return 0;
}
