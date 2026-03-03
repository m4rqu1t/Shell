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



    }
    return 0;
}
