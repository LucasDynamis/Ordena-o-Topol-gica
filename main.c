#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_DISCIPLINAS 100

typedef struct Vertice {
    char disciplina[100];
    struct Vertice* destinos[MAX_DISCIPLINAS];
    int num_destinos;
    int visitado;
} Vertice;

typedef struct Grafo {
    Vertice vertices[MAX_DISCIPLINAS];
    int num_disciplinas;
} Grafo;

typedef struct TipoCelula {
    int item;
    struct TipoCelula* proximo;
} TipoCelula;

typedef struct TipoLista {
    TipoCelula* primeiro;
    TipoCelula* ultimo;
} TipoLista;

void inicializarGrafo(Grafo* grafo);
void adicionarDisciplina(Grafo* grafo, const char* disciplina);
void adicionarDestino(Vertice* vertice, Vertice* destino);
void adicionarAresta(Grafo* grafo, int origem, int destino);
void buscaEmProfundidade(Grafo* grafo, TipoLista* lista);
void visitaDFS(Vertice* vertice, Grafo* grafo, TipoLista* lista);
void criarListaVazia(TipoLista* lista);
void inserirLista(int item, TipoLista* lista);
void ordenacaoTopologica(Grafo* grafo);

int main() {
    Grafo grade;
    inicializarGrafo(&grade);

    FILE* arquivo = fopen("codigos_disciplinas.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    char linha[100];
    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = '\0'; // Remover o caractere de nova linha
        if (linha[0] != '\0') {
            if (isdigit((unsigned char)linha[0])) {
                int origem, destino;
                if (sscanf(linha, "%d %d", &origem, &destino) == 2) {
                    adicionarAresta(&grade, origem, destino);
                }
            } else {
                adicionarDisciplina(&grade, linha);
            }
        }
    }

    fclose(arquivo);

    ordenacaoTopologica(&grade);

    return 0;
}

void inicializarGrafo(Grafo* grafo) {
    grafo->num_disciplinas = 0;
    for (int i = 0; i < MAX_DISCIPLINAS; i++) {
        grafo->vertices[i].num_destinos = 0;
        grafo->vertices[i].visitado = 0;
    }
}

void adicionarDisciplina(Grafo* grafo, const char* disciplina) {
    if (grafo->num_disciplinas < MAX_DISCIPLINAS) {
        Vertice* vertice = &grafo->vertices[grafo->num_disciplinas++];
        strncpy(vertice->disciplina, disciplina, sizeof(vertice->disciplina) - 1);
        vertice->disciplina[sizeof(vertice->disciplina) - 1] = '\0';
        vertice->visitado = 0;
        vertice->num_destinos = 0;
    }
}

void adicionarDestino(Vertice* vertice, Vertice* destino) {
    if (vertice->num_destinos < MAX_DISCIPLINAS) {
        vertice->destinos[vertice->num_destinos++] = destino;
    }
}

void adicionarAresta(Grafo* grafo, int origem, int destino) {
    if (origem >= 0 && origem < grafo->num_disciplinas && destino >= 0 && destino < grafo->num_disciplinas) {
        adicionarDestino(&grafo->vertices[origem], &grafo->vertices[destino]);
    }
}

void buscaEmProfundidade(Grafo* grafo, TipoLista* lista) {
    for (int u = 0; u < grafo->num_disciplinas; u++) {
        if (!grafo->vertices[u].visitado) {
            visitaDFS(&grafo->vertices[u], grafo, lista);
        }
    }
}

void visitaDFS(Vertice* vertice, Grafo* grafo, TipoLista* lista) {
    vertice->visitado = 1;
    for (int i = 0; i < vertice->num_destinos; i++) {
        Vertice* destino = vertice->destinos[i];
        if (!destino->visitado) {
            visitaDFS(destino, grafo, lista);
        }
    }
    inserirLista(vertice - grafo->vertices, lista);
}

void criarListaVazia(TipoLista* lista) {
    lista->primeiro = NULL;
    lista->ultimo = NULL;
}

void inserirLista(int item, TipoLista* lista) {
    TipoCelula* novaCelula = malloc(sizeof(TipoCelula));
    novaCelula->item = item;
    novaCelula->proximo = NULL;

    if (lista->primeiro == NULL) {
        lista->primeiro = novaCelula;
        lista->ultimo = novaCelula;
    } else {
        lista->ultimo->proximo = novaCelula;
        lista->ultimo = novaCelula;
    }
}

void ordenacaoTopologica(Grafo* grafo) {
    TipoLista lista;
    criarListaVazia(&lista);

    buscaEmProfundidade(grafo, &lista);

    printf("Ordenacao topologica disciplinas:\n\n");

    TipoCelula* celula = lista.primeiro;
    while (celula != NULL) {
        printf("%s", grafo->vertices[celula->item].disciplina);
        if (celula->proximo != NULL) {
            printf(" --> ");
        }
        celula = celula->proximo;
    }
    printf("\n");

    TipoCelula* atual = lista.primeiro;
    while (atual != NULL) {
        TipoCelula* temp = atual;
        atual = atual->proximo;
        free(temp);
    }
}
