#include <stdio.h>
#include "lista.c"

int compare_to(void* a, void* b) {
    int v1 = *(int*)a;
    int v2 = *(int*)b;

    if (v1 < v2) {
        return -1;
    }

    if (v1 == v2) {
        return 0;
    }

    return 1;
}

int main() {
    Lista lis = lista_new();

    int i=5;
    lista_inserir_ordenado(&lis, (void*)(&i), sizeof(int), &compare_to);
    i=4;
    lista_inserir_ordenado(&lis, (void*)(&i), sizeof(int), &compare_to);
    i=3;
    lista_inserir_ordenado(&lis, (void*)(&i), sizeof(int), &compare_to);
    i=2;
    lista_inserir_ordenado(&lis, (void*)(&i), sizeof(int), &compare_to);
    i=1;
    lista_inserir_ordenado(&lis, (void*)(&i), sizeof(int), &compare_to);

    No* atual = lis.prim;
    while (atual != NULL) {
        printf("%d\n", *(int*)atual->info);
        atual = atual->prox;
    }

    printf("%d\n", strcmp("aa", "bb"));

    return 0;
}