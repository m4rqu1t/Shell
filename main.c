#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(){
    while(1){

        printf("Shell BRABO>");
        char input[1001];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nFECHANDO  \n");
            exit(0);
        }

        int i = 0;
        while (input[i] != '\n' && input[i] != '\0') {
            i++;
        }
        if (input[i] == '\n') {
            input[i] = '\0';
        }

        char *args[1001];
        int j = 0;
        char *p = input;
        while(*p!='\0'){
            while(*p ==' '){
                *p = '\0';
                p++;
            }
            if(*p == '\0'){
                break;
            }
            args[j]=p;
            j++;
            while(*p!= ' ' && *p != '\0'){
                p++;
            }
        }
        args[j] = NULL;

        if (args[0] == NULL) {
            continue;
        }

        int pid = fork();

        if(pid<0){
            printf("ERRO NO FORK\n");
        }else if(pid == 0){
            if(execvp(args[0], args) == -1){
                printf("NAO EXISTE O COMANDO %s\n",args[0]);
                exit(1);
            }
        }else{
            int status;
            waitpid(pid, &status, 0);
        }

    }
    return 0;
}
