#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_NOMEARQ 256
#define ENTRE(valor, min, max) (min <= valor && valor <= max)



/* =========================================================== */
/*                            LISTA                            */
/* =========================================================== */
/* Sera transferida para uma biblioteca posteriormente */

typedef
    struct No {
        void* info;
        size_t tamanho;
        struct No* prox;
    } No;

void inicializa_no(No* n, void* info, size_t tamanho, No* prox) {
    n->info    = info;
    n->tamanho = tamanho;
    n->prox    = prox;
}

typedef
    struct {
        No* prim;
        No* ult;
    } Lista;

Lista new_lista() {
    Lista ret;
    ret.prim = NULL;
    ret.ult   = NULL;
    return ret;
}

void inserir_final_lista(Lista* lis, void* valor, size_t tamanho_valor) {
    /* alocacao de memoria do que sera inserido na lista */
    void* copia = malloc(tamanho_valor);
    memcpy(copia, valor, tamanho_valor);
    No* novo = (No*)malloc(sizeof(No));

    inicializa_no(novo, copia, tamanho_valor, NULL);

    if (lis->prim == NULL) {
        lis->prim = novo;
        lis->ult  = novo;
        return;
    }

    lis->ult->prox = novo;
    lis->ult = novo;
}


/* =========================================================== */
/*                 FUNCOES E STRUCTS AUXILIARES                */
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

    return EXIT_SUCCESS;
}



