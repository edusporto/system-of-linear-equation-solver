#ifndef _LISTA_H_
#define _LISTA_H_

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
        } Lista;

#endif
