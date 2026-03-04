#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

int ler_e_cortar(char *input, char **args, int *background){

    *background = 0;

    if(fgets(input, 1001, stdin) == NULL){
        printf("\nFECHANDO\n");
        exit(0);
    }

    int i = 0;
        while (input[i] != '\n' && input[i] != '\0'){
            i++;
        }
        if (input[i] == '\n'){
             input[i] = '\0';
        }

        int j = 0;
            char *p = input;
            while(*p != '\0'){
                while(*p == ' '){
                *p = '\0'; p++; }
                if(*p == '\0') break;
                args[j] = p;
                j++;
                while(*p != ' ' && *p != '\0') p++;
            }
        args[j] = NULL;

        if(args[0] == NULL){
            return 0;
        }

        if(j>0 && strcmp(args[j-1], "&") == 0){
            *background = 1;
            args[j-1] = NULL;
        }
        return 1;
}
