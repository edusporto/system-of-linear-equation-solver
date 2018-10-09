#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_NOMEARQ 256
#define ENTRE(valor, min, max) (min <= valor && valor <= max)




/* =========================================================== */
/*                            LISTA                            */
/* =========================================================== */
/*     Sera transferida para uma biblioteca posteriormente     */

typedef
    struct No {
        void* info;
        size_t tamanho;
        struct No* prox;
    } No;

void no_inicializa(No* n, void* info, size_t tamanho, No* prox) {
    n->info    = info;
    n->tamanho = tamanho;
    n->prox    = prox;
}

typedef
    struct {
        No* prim;
        No* ult;
    } Lista;

Lista lista_new() {
    Lista ret;
    ret.prim = NULL;
    ret.ult   = NULL;
    return ret;
}

void lista_inserir_fim(Lista* lis, void* valor, size_t tamanho_valor) {
    /* alocacao de memoria do que sera inserido na lista */
    void* copia = malloc(tamanho_valor);
    memcpy(copia, valor, tamanho_valor);
    No* novo = (No*)malloc(sizeof(No));

    no_inicializa(novo, copia, tamanho_valor, NULL);

    if (lis->prim == NULL) {
        lis->prim = novo;
        lis->ult  = novo;
        return;
    }

    lis->ult->prox = novo;
    lis->ult = novo;
}

void lista_inserir_inicio(Lista* lis, void* valor, size_t tamanho_valor) {
    /* alocacao de memoria do que sera inserido na lista */
    void* copia = malloc(tamanho_valor);
    memcpy(copia, valor, tamanho_valor);
    No* novo = (No*)malloc(sizeof(No));

    no_inicializa(novo, copia, tamanho_valor, lis->prim);

    if (lis->prim == NULL)
        lis->ult = novo;

    lis->prim = novo;
}

/* retorna NULL caso o index passado como parametro seja
 * maior que a qtd de itens da lista */
void* lista_get_item(Lista* lis, int index) {
    int i;
    No* atual = lis->prim;

    for (i=0; i<index; i++) {
        if (atual == NULL)
            return NULL;
        atual = atual->prox;
    }

    if (atual != NULL)
        return atual->info;
    return NULL;
}

char lista_remover_primeiro(Lista* lis) {
    No* atual;

    if (lis->prim == NULL)
        return -1;

    if (lis->prim == lis->ult)
        lis->ult = NULL;

    atual = lis->prim;

    lis->prim = atual->prox;
    free(atual->info);
    free(atual);
    return 1;
}

char lista_remover_ultimo(Lista* lis) {
    No* atual;
    No* anterior;

    if (lis->prim == NULL)
        return -1;

    if (lis->prim->prox == NULL) {
        free(lis->prim->info);
        free(lis->prim);
        lis->prim = NULL;
        lis->ult  = NULL;
        return 1;
    }

    atual    = lis->prim;
    anterior = NULL;

    while (atual != lis->ult) {
        anterior = atual;
        atual    = atual->prox;
    }

    anterior->prox = NULL;
    lis->ult = anterior;
    free(atual->info);
    free(atual);

    return 1;
}

/* retorna  1 em caso de sucesso
 * retorna -1 em caso de falha   */
char lista_remover_item(Lista* lis, int index) {
    No* atual    = lis->prim;
    No* anterior = NULL;
    int i;

    if (index < 0)
        return -1;

    if (index == 0)
        return lista_remover_primeiro(lis);

    for (i=0; i<index; i++) {
        if (atual == NULL)
            return -1;
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == lis->ult)
        lis->ult = anterior;

    anterior->prox = atual->prox;
    free(atual->info);
    free(atual);

    return 1;
}






/* =========================================================== */
/*                 FUNCOES E STRUCTS AUXILIARES                */
/* =========================================================== */

typedef
    struct {
        char* nome;
        size_t tamanho_nome;
        double coeficiente;
    } Incognita;

Incognita incognita_new(char* nome, size_t tamanho_nome, double coeficiente) {
    Incognite ret = {nome, tamanho_nome, coeficiente};
    return ret;
}

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
    char linha[1024];
    char buffer[1024];
    char caracterEmString[2];
    char atual;
    //double numeroAtual;
    double** m_coeficientes, m_termosIndependentes;
    Lista* equacoes; /* eh um vetor */
    int qtdEquacoes, i, j;
    FILE* arq;
    Incognita incognita_atual;

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

        /* ALOCANDO MEMORIA PARA O VETOR DE LISTA QUE GUARDA AS INCOGNITAS */
        equacoes = (Lista*)malloc(sizeof(Lista) * qtdEquacoes);

        for (i=0; i<qtdEquacoes; i++) {
            *(buffer) = '\0';
            *(linha)  = '\0';

            while(atual = fgetc(arq), atual != '\n' && !feof(arq)) {
                if (atual == ' ')
                    continue;

                *(caracterEmString)     = atual;
                *(caracterEmString + 1) = '\0';

                strncat(linha, caracterEmString, 1024 - strlen(linha));
            }

            /* valores possiveis de incognita atual:
             * nome == NULL, caso esteja sendo lida uma incognita nova
             * nome == NULL + 1, caso um termo independente esteja sendo lido
             * nome == algo, caso tenha terminado de ler o nome.
                |-> Devera virar NULL ou NULL + 1 */
            incognita_atual = incognita_new(NULL, 0, 0);

            int tamanho_linha = strlen(linha);
            for (j=0; j<tamanho_linha; j++) {
                atual = *(linha + j);

                if (ENTRE(atual, '0', '9') || atual == '.' ||
                    atual == '+'           || atual == '-') {

                    if (incognita_atual.nome == NULL || incognita_atual.nome == NULL + 1) {
                        /* o coeficiente de uma incognita ou termo independente esta sendo lido */
                        *(caracterEmString)     = atual;
                        strncat(buffer, caracterEmString, 1024 - strlen(buffer));
                        continue;
                    } else {
                        /* o coeficiente de uma nova incognite esta sendo lido */
                        int tamanho_nome     = strlen(buffer)
                        incognita_atual.nome = (char*)malloc(tamanho_linha+1);
                        strcpy(incognita_atual.nome, buffer);
                        lista_inserir_fim(equacoes, (void*)(&incognita_atual), sizeof(incognita_atual));
                        *(buffer) = '\0';

                        *(caracterEmString)     = atual;
                        strncat(buffer, caracterEmString, 1024 - strlen(buffer));
                    }
                }

                /* FALTA FAZER CASO ESTEJA SENDO LIDO UM NOME OU O CARACTER DE IGUAL DA EQUACAO */

            }

        }

            // METODO ANTIGO
            // while (atual = fgetc(arq),
            //        ENTRE(atual, '0', '9') ||
            //        atual == '.') {
            //     /* caso o programa esteja aqui, um numero
            //      * esta sendo lido */

            //     *(caracterEmString)     = atual;
            //     *(caracterEmString + 1) = '\0';
            //     strncat(buffer, caracterEmString, 1024 - strlen(buffer));
            // }

            // if (*buffer == '\0')
            //     numeroAtual = 1;
            // else
            //     numeroAtual = atof(buffer);
            /* quando o programa chega aqui, deve-se ler o nome da variavel do numero lido */


//            OUTRO METODO ANTIGO
//            atual = fgetc(arq);
//            while (atual != '\n' && !feof(arq)) {
//                *(buffer) = '\0';
//                if (ENTRE(atual, '0', '9') || atual == '-' || atual == '+') {
//                    /* esta lendo um numero */
//                    ungetc(atual, arq);
//                    fscanf(arq, "%lf", &numeroAtual);
//                    printf("%lf\n", numeroAtual);
//                    atual = fgetc(arq);
//                }
//                else
//                if (atual != '=') {
//                    /* esta lendo um nome de variavel */
//                    ungetc(atual, arq);
//                    while (atual = fgetc(arq), atual != '+' && atual != '-' && atual != '=') {
//                        *(caracterEmString)     = atual;
//                        *(caracterEmString + 1) = '\0';
//                        strncat(buffer, caracterEmString, 1024 - strlen(buffer));
//                        printf("%s", buffer);
//                    }
//                }
//
//            }


        //printf("%lf %s", numeroAtual, buffer);



        /* LEMBRAR DE DESALOCAR MEMORIA */
        free(equacoes);

        fclose(arq);

    } while (terminar() != 'S');

    return EXIT_SUCCESS;
}



