#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_NOMEARQ 256
#define ENTRE(valor, min, max) (min <= valor && valor <= max)


/* =========================================================== */
/*                     FUNCOES AUXILIARES                      */
/* =========================================================== */

char terminar() {
    char resp;
    printf("\nDeseja parar o programa? (S/N)\n");
    scanf(" %c", &resp);
    return toupper(resp);
}


/* =========================================================== */
/*               FUNCOES PRINCIPAIS E FUNCAO MAIN              */
/* =========================================================== */

int main(void) {
    char nomeArq[MAX_NOMEARQ];
    char buffer[1024];
    char caracterEmString[2];
    char atual;
    int qtdEquacoes, i, j;
    FILE* arq;

    *(caracterEmString+1) = '\0'; /*usado posteriormente para concatenar chars a strings */

    printf("=============================================\n");
    printf(" Resolvedor de sistemas de equacoes lineares \n");
    printf("=============================================\n");

    do {
        printf("\nDigite o nome do arquivo contendo um sistema de equacoes lineares:\n");
        scanf("%255s", nomeArq);

        if ((arq = fopen(nomeArq, "r")) == NULL) {
            /* o programa nao conseguiu abrir o arquivo */
            printf("\nNao foi possivel abrir este arquivo.\n");
            continue;
        }

        /* Lendo a quantidade de equacoes a ler */
        fscanf(arq, "%d", &qtdEquacoes);
        fgetc(arq); /* Le o caracter de nova linha */

        double teste;
        for (i=0; i<qtdEquacoes; i++) {
            *(buffer) = '\0';

            while (atual = fgetc(arq),
                   ENTRE(atual, '0', '9') ||
                   atual == '.') {
                /* caso o programa esteja aqui, um numero
                 * esta sendo lido */

                *(caracterEmString)     = atual;
                *(caracterEmString + 1) = '\0';
                strcat(buffer, caracterEmString);
            }

            /* TODO: GUARDAR A VARIAVEL E SEU VALOR EM UM STRUCT DENTRO DE UMA LISTA */
            teste = atof(buffer);
        }

        printf("%lf %s", teste, buffer);


        fclose(arq);

    } while (terminar() != 'S');

    return 0;
}



