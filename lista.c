#include <stdlib.h>
#include <string.h>
#include "lista.h"

/* =========================================================== */
/*                            LISTA                            */
/* =========================================================== */

void no_inicializa(No* n, void* info, size_t tamanho, No* prox) {
    n->info    = info;
    n->tamanho = tamanho;
    n->prox    = prox;
}

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