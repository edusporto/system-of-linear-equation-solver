#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lista.c"

#define MAX_NOMEARQ 256
#define ENTRE(valor, min, max) (min <= valor && valor <= max)


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
    Incognita ret = {nome, tamanho_nome, coeficiente};
    return ret;
}

char terminar() {
    char resp;
    printf("\nDeseja parar o programa? (S/N)\n");
    scanf(" %c", &resp);
    return toupper(resp);
}

int compare_to(void* string1, void* string2) {
    return strcmp((char*)string1, (char*)string2);
}

double determinante(double** matriz, int ordem) {
	int    a, i, j;
	double ret = 0;
	int    c1, c2;
	int    O = 1; /* representa se o cofator atual deve ser multiplicado por 1 ou -1 */

	if (ordem == 2) {
		ret = **(matriz) * *(*(matriz+1)+1) - *(*(matriz)+1) * *(*(matriz+1));
		return ret;
	}

	double** matriz_menor = (double**)malloc(sizeof(double*)*(ordem-1));
	for (i=0; i<ordem-1; i++)
		*(matriz_menor+i) = (double*)malloc(sizeof(double)*(ordem-1));

	for (a=0; a<ordem; a++) { /* ira percorrer a primeira linha da matriz */
		c1 = 0, c2 = 0;
		for (i=1; i<ordem; i++) { /* linha da matriz menor */
			for (j=0; j<ordem; j++) { /* coluna da matriz menor */
				if (j != a) {
					/* ira colocar na matriz menor somente aqueles que nao pertencem
					 * a linha e coluna do elemento cujo cofator esteja sendo calculado */
					*(*(matriz_menor+c1)+c2) = *(*(matriz+i)+j);
					c2++;
					if (c2>ordem-2) {
						c1++;
						c2=0;
					}
				}
			}
		}
		ret = ret + O*(*(*(matriz)+a) * determinante(matriz_menor, ordem-1));
		O = -1*O;
	}

	for (i=0; i<(ordem-1); i++)
		free(*(matriz_menor+i));
	free(matriz_menor);

	return ret;
}

/* =========================================================== */
/*               FUNCOES PRINCIPAIS E FUNCAO MAIN              */
/* =========================================================== */

/* RETORNA UM VETOR DE LISTA, CADA LISTA CONTENDO UMA EQUACAO */
Lista* ler_equacoes (FILE* arq, int qtdEquacoes) {
    char linha[2048];
    char buffer[1024];
    char caracterEmString[2];
    char atual;
    Lista* equacoes; /* sera o retorno da funcao */
    int i, j;
    Incognita incognita_atual;

    *(caracterEmString+1) = '\0'; /*usado posteriormente para concatenar chars a strings */

    /* Aloca memoria para o vetor de equacoes */
    equacoes = (Lista*)malloc(sizeof(Lista) * qtdEquacoes);
    /* Instancia as equacoes do vetor */
    for (i=0; i<qtdEquacoes; i++) {
        *(equacoes + i) = lista_new();
    }

    for (i=0; i<qtdEquacoes; i++) {
        *(buffer) = '\0';
        *(linha)  = '\0';

        /* Escreve a equacao atual em uma string, porem, sem os espacos */
        while (atual = fgetc(arq), atual != '\n' && !feof(arq)) {
            if (atual == ' ')
                continue;

            *(caracterEmString) = atual;

            strncat(linha, caracterEmString, 2048 - strlen(linha));
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

                /* O PROGRAMA ESTA LENDO UM COEFICIENTE OU O TERMO INDEPENDENTE */

                if (incognita_atual.nome == NULL || incognita_atual.nome == NULL + 1) {
                    /* o coeficiente de uma incognita ou termo independente esta sendo lido */
                    *(caracterEmString) = atual;
                    strncat(buffer, caracterEmString, 1024 - strlen(buffer));
                    continue;
                } else {
                    /* o coeficiente de uma nova incognite esta sendo lido */
                    int tamanho_nome             = strlen(buffer);
                    incognita_atual.nome         = (char*)malloc(tamanho_nome+1);
                    incognita_atual.tamanho_nome = tamanho_nome+1;
                    strcpy(incognita_atual.nome, buffer);
                    lista_inserir_fim(equacoes+i, (void*)(&incognita_atual), sizeof(incognita_atual));
                    *(buffer) = '\0';
                    incognita_atual.nome = NULL;

                    *(caracterEmString)     = atual;
                    strncat(buffer, caracterEmString, 1024 - strlen(buffer));
                    continue;
                }
            }

            if (atual == '=') {
                /* O PROGRAMA ESTA LENDO O INICIO DE UM TERMO INDEPENDENTE */
                int tamanho_nome             = strlen(buffer);
                incognita_atual.nome         = (char*)malloc(tamanho_nome+1);
                incognita_atual.tamanho_nome = tamanho_nome+1;
                strcpy(incognita_atual.nome, buffer);
                lista_inserir_fim(equacoes+i, (void*)(&incognita_atual), sizeof(incognita_atual));
                *(buffer) = '\0';
                incognita_atual.nome = NULL + 1;
                continue;
            }

            /* O PROGRAMA ESTA LENDO O NOME DE UMA INCOGNITA */

            if (*(buffer) == '\0' ||
                (*(buffer) == '+' && *(buffer+1) == '\0')) {
                /* nao ha numero indicando o coeficiente da incognita sendo lida */
                /*incognita_atual.coeficiente = 1;*/
                *(buffer)   = '1';
                *(buffer+1) = '\0';
            }

            if (*(buffer) == '-' && *(buffer+1) == '\0') {
                /* nao ha numero indicando o coeficiente da incognita sendo lida */
                /* porem, ha um menos indicando que o coeficiente eh negativo */
                /*incognita_atual.coeficiente = -1;*/
                *(buffer) = '-';
                *(buffer+1) = '1';
                *(buffer+2) = '\0';
            }

            if (ENTRE(*(buffer), '0', '9') || *(buffer) == '+' || *(buffer) == '-') {
                /* verifica se um numero estava sendo lido antes do nome */
                incognita_atual.coeficiente = atof(buffer);
                *(buffer) = '\0';
            }

            incognita_atual.nome        = buffer;
            *(caracterEmString) = atual;
            strncat(buffer, caracterEmString, 1024 - strlen(buffer));
        }

        /* configurando o termo independente pra ser inserido na equacao */
        /* o nome do termo independente sera " ", um espaco em branco */
        char* nomeFinal = (char*)malloc(sizeof(char) * 2);
        strcpy(nomeFinal, " ");
        incognita_atual.nome         = nomeFinal;
        incognita_atual.tamanho_nome = sizeof(char) * 2;
        incognita_atual.coeficiente  = atof(buffer);

        /* ja foi lida toda a equacao, falta guardar o termo independente */
        lista_inserir_fim(equacoes+i, (void*)(&incognita_atual), sizeof(incognita_atual));
    }

    return equacoes;
}



int main(void) {
    char nomeArq[MAX_NOMEARQ];
    double** m_coeficientes;
    double*  m_termosIndependentes;
    Lista* equacoes; /* eh um vetor */
    Lista  nome_incognitas;
    int i, j, qtdEquacoes;
    FILE* arq;
    No* no_atual;

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

        /* a memoria para este vetor eh dentro da funcao */
        equacoes = ler_equacoes(arq, qtdEquacoes);

        /* O arquivo nao eh mais necessario, pode ser fechado */
        fclose(arq);

        nome_incognitas = lista_new();
        /* todas os nomes de incognitas agora serao colocados em uma lista */
        for (i=0; i<qtdEquacoes; i++) {
            no_atual = (equacoes+i)->prim;
            while (no_atual != NULL) {
                lista_inserir_ordenado(&nome_incognitas,
                                       (void*)(((Incognita*)no_atual->info)->nome),
                                       ((Incognita*)no_atual->info)->tamanho_nome,
                                       compare_to);
                no_atual = no_atual->prox;
            }
        }


        /*
        ESCREVE O NOME DE TODAS AS INCOGNITAS
        No* aa;
        aa = nome_incognitas.prim;
        while (aa != NULL) {
            printf("%s\n", (char*)aa->info);
            aa = aa->prox;
        }

        ESCREVE AS INCOGNITAS DE TODAS AS EQUACOES
        No* at;
        for (i=0; i<qtdEquacoes; i++) {
            at = equacoes[i].prim;
            while (at != NULL) {
                printf("%.3lf, %s, %lu\n", ((Incognita*)(at->info))->coeficiente, ((Incognita*)(at->info))->nome, ((Incognita*)(at->info))->tamanho_nome  );
                at = at->prox;
            }
        }
        */

        /* sera verificado se o sistema eh valido, e serao instanciadas a matriz
         * de coeficiente e a matriz de termos independentes */
        if (nome_incognitas.qtd-1 != qtdEquacoes) {
            printf("\nEste sistema nao e um SPD (Sistema Possivel Determinado)\n");
        } else {
            /* instanciando a matriz de coeficientes e a de termos independentes */
            m_coeficientes = (double**)malloc(sizeof(double*) * qtdEquacoes);
            for (i=0; i<qtdEquacoes; i++) {
                *(m_coeficientes+i) = (double*)malloc(sizeof(double) * (qtdEquacoes));
                for (j=0; j<qtdEquacoes; j++) {
                    /* colocando o valor inicial de cada posicao da matriz de
                     * coeficientes como 0 */
                    *(*(m_coeficientes+i) + j) = 0;
                }
            }
            m_termosIndependentes = (double*)malloc(sizeof(double) * qtdEquacoes);

            No* no_aux;
            Incognita* incognita_atual;
            for (i=0; i<qtdEquacoes; i++) {
                no_atual = (equacoes+i)->prim;
                while (no_atual != NULL) {
                    incognita_atual = ((Incognita*)no_atual->info);
                    if (strcmp(incognita_atual->nome, " ") == 0) {
                        /* esta lendo um termo independente */
                        *(m_termosIndependentes+i) = incognita_atual->coeficiente;
                        no_atual = no_atual->prox;
                        continue;
                    }

                    j=-1;
                    no_aux = nome_incognitas.prim;
                    while (no_aux != NULL) {
                        /* esta lendo o nome de uma variavel para guardar seu coeficiente
                         * na matriz de coeficientes;
                        char* incognita_procurando = (char*)no_aux->info;
                        char* incognita_atual = ((Incognita*)no_atual->info)->nome;
                        if (strcmp(incognita_procurando, incognita_atual) == 0) {
                            *(*(m_coeficientes+i)+j) = ((Incognita*)no_atual->info)->coeficiente;
                            break;
                        }
                        j++;
                        no_aux = no_aux->prox;
                    }
                    no_atual = no_atual->prox;
                }
            }

            /*
            printf("\nMatriz de coeficientes:\n");
            for (i=0; i<qtdEquacoes; i++) {
                for (j=0; j<nome_incognitas.qtd-1; j++) {
                    printf("%.3lf ", m_coeficientes[i][j]);
                }
                printf("\n");
            }

            printf("\nMatriz dos termos independentes:\n");
            for (i=0; i<qtdEquacoes; i++) {
                printf("%lf\n", m_termosIndependentes[i]);
            }
            */

            /* A PARTIR DAQUI, O RESULTADO DO SISTEMA SERA CALCULADO */
            double det_m_coeficientes = determinante(m_coeficientes, qtdEquacoes);

            if (det_m_coeficientes == 0) {
                printf("\nEste sistema nao e um SPD (Sistema Possivel Determinado)\n");
            } else {
                int a;
                double** m_temp = (double**)malloc(sizeof(double*) * qtdEquacoes);
                for (i=0; i<qtdEquacoes; i++)
                    *(m_temp+i) = (double*)malloc(sizeof(double) * qtdEquacoes);

                no_atual = nome_incognitas.prim;
                no_atual = no_atual->prox; /* o primeiro valor eh o termo independente */
                printf("\nRESULTADO:\n");
                /* ESCREVENDO A MATRIZ COM OS TERMOS INDEPENDENTES NO LUGAR DA COLUNA
                 * DA INCOGNITA ATUAL, A QUAL ESTA SENDO CALCULADA A SOLUCAO */
                for (a=0; a<qtdEquacoes; a++) {
                    for (i=0; i<qtdEquacoes; i++) {
                        for (j=0; j<qtdEquacoes; j++) {
                            if (j==a)
                                *(*(m_temp+i)+j) = *(m_termosIndependentes+i);
                            else
                                *(*(m_temp+i)+j) = *(*(m_coeficientes+i)+j);
                        }
                    }

                    /* NOME DA INCOGNITA ATUAL */
                    printf("%s = ", (char*)no_atual->info);
                    /* RESULTADO DA INCOGNITA ATUAL */
                    printf("%.3lf\n", determinante(m_temp, qtdEquacoes) / det_m_coeficientes);

                    no_atual = no_atual->prox;
                }

                for (i=0; i<qtdEquacoes; i++)
                    free(*(m_temp+i));
                free(m_temp);
            }

            /* LIBERACAO DE MEMORIA - MATRIZES */
            for (i=0; i<qtdEquacoes; i++) {
                free(*(m_coeficientes+i));
            }
            free(m_coeficientes);
            free(m_termosIndependentes);
        }

        /* LIBERACAO DE MEMORIA */
        Lista     lis_atual;
        Incognita inc_atual;
        No*       no_atual;
        fim: for (i=0; i<qtdEquacoes; i++) {
            lis_atual = *(equacoes+i);
            no_atual = lis_atual.prim;
            while (no_atual != NULL) {
                inc_atual = *(Incognita*)no_atual->info;
                free(inc_atual.nome);
                no_atual = no_atual->prox;
            }
            while (lista_remover_primeiro(&lis_atual) != -1)
                ;
        }
        free(equacoes);

        while(lista_remover_primeiro(&nome_incognitas) != -1)
            ;

    } while (terminar() != 'S');

    return EXIT_SUCCESS;
}



