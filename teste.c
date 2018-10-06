#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

/* retorna NULL caso o index passado como parametro seja
 * maior que a qtd de itens da lista */
void* get_item_lista(Lista* lis, int index) {
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


int main() {
    Lista lis = new_lista();

    int i=4;
    inserir_final_lista(&lis, (void*)&i, sizeof(int));
    i = 2;
    inserir_final_lista(&lis, (void*)&i, sizeof(int));
    i = 3;
    inserir_final_lista(&lis, (void*)&i, sizeof(int));

    printf("%d, %d, %d", *(int*)get_item_lista(&lis, 0), *(int*)get_item_lista(&lis, 1), *(int*)get_item_lista(&lis, 2));

    return EXIT_SUCCESS;
}