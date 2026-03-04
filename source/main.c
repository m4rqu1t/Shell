#include <stdio.h>
#include "parser.h"
#include "executor.h"

int main() {
    char input[1001];
    char *args[1001];
    int background;

    while(1) {
        printf("Shell BRABO> ");

        if (ler_e_cortar(input, args, &background)) {
            executar(args, background);
        }
    }

    return 0;
}
