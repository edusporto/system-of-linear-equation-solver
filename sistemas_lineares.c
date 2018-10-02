#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_NOMEARQ 256

typedef
    struct {
        char* valor;
        int tamanhoAlocado;
    } string;

/* FAZER AS FUNCOES DE STRING */

/*string nova_string()*/

/* aloca a string em outra parte da memoria com mais espaco */
/* retorna NULL caso ocorra algum erro */
/* caso contrario, retorna o novo endereco de memoria da string */
/*char* aumentar_string(string str, float taxa_crescimento) {
    if (taxa_crescimento <= 0 )
        return NULL;

}*/

char terminar() {
    char resp;
    printf("\nDeseja parar o programa? (S/N)\n");
    scanf(" %c", &resp);
    return toupper(resp);
}

// 2x + 3yy + z + 44s = 2345
int main(void) {
    char nomeArq[MAX_NOMEARQ];
    int qtdEquacoes;
    FILE* arq;

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



        fclose(arq);

    } while (terminar() != 'S');

    return 0;
}



