#ifndef LISTA_H
#define LISTA_H
#include <stdlib.h>

    typedef
        struct No {
            void* info;
            size_t tamanho;
            struct No* prox;
        } No;

    typedef
        struct {
            No* prim;
            No* ult;
            int qtd;
        } Lista;

    void no_inicializa(No* n, void* info, size_t tamanho, No* prox);

    Lista lista_new();

    void lista_inserir_fim(Lista* lis, void* valor, size_t tamanho_valor);

    void lista_inserir_inicio(Lista* lis, void* valor, size_t tamanho_valor);

    void* lista_get_item(Lista* lis, int index);

    char lista_remover_primeiro(Lista* lis);

    char lista_remover_ultimo(Lista* lis);

    char lista_remover_item(Lista* lis, int index);

    void lista_inserir_ordenado (Lista* lis, void* valor, size_t tamanho_valor, int (*compareTo) (void*, void*));

#endif
