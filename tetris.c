#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

typedef struct {
    char nome;
    int id;
} Peca;

typedef struct {
    Peca pecas[TAM_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
} Pilha;

void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->quantidade = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

int filaCheia(Fila *f) {
    return f->quantidade == TAM_FILA;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

void enfileirar(Fila *f, Peca nova) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % TAM_FILA;
    f->pecas[f->tras] = nova;
    f->quantidade++;
}

Peca desenfileirar(Fila *f) {
    Peca removida = {'-', -1};
    if (filaVazia(f)) return removida;
    removida = f->pecas[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->quantidade--;
    return removida;
}

void empilhar(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) return;
    p->pecas[++p->topo] = nova;
}

Peca desempilhar(Pilha *p) {
    Peca removida = {'-', -1};
    if (pilhaVazia(p)) return removida;
    removida = p->pecas[p->topo--];
    return removida;
}

void trocarTopo(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) return;
    Peca temp = f->pecas[f->frente];
    f->pecas[f->frente] = p->pecas[p->topo];
    p->pecas[p->topo] = temp;
}

void trocaMultipla(Fila *f, Pilha *p) {
    if (f->quantidade < 3 || p->topo < 2) return;
    for (int i = 0; i < 3; i++) {
        int indiceFila = (f->frente + i) % TAM_FILA;
        Peca temp = f->pecas[indiceFila];
        f->pecas[indiceFila] = p->pecas[p->topo - i];
        p->pecas[p->topo - i] = temp;
    }
}

void exibirEstado(Fila *f, Pilha *p) {
    printf("\nFila de peças: ");
    for (int i = 0; i < f->quantidade; i++) {
        int indice = (f->frente + i) % TAM_FILA;
        printf("[%c %d] ", f->pecas[indice].nome, f->pecas[indice].id);
    }
    printf("\nPilha de reserva (Topo -> Base): ");
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
    }
    printf("\n");
}

int main() {
    Fila fila;
    Pilha pilha;
    int idAtual = 0, opcao;
    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    for (int i = 0; i < TAM_FILA; i++)
        enfileirar(&fila, gerarPeca(idAtual++));

    do {
        exibirEstado(&fila, &pilha);
        printf("\n1 - Jogar peça\n2 - Reservar peça\n3 - Usar peça reservada\n4 - Trocar peça atual\n5 - Trocar 3 primeiras\n0 - Sair\nEscolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                if (!filaVazia(&fila)) {
                    desenfileirar(&fila);
                    enfileirar(&fila, gerarPeca(idAtual++));
                }
                break;
            }
            case 2: {
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca peca = desenfileirar(&fila);
                    empilhar(&pilha, peca);
                    enfileirar(&fila, gerarPeca(idAtual++));
                }
                break;
            }
            case 3: {
                if (!pilhaVazia(&pilha)) {
                    desempilhar(&pilha);
                    enfileirar(&fila, gerarPeca(idAtual++));
                }
                break;
            }
            case 4:
                trocarTopo(&fila, &pilha);
                break;
            case 5:
                trocaMultipla(&fila, &pilha);
                break;
            case 0:
                printf("\nEncerrando...\n");
                break;
            default:
                printf("\nOpção inválida.\n");
        }
    } while (opcao != 0);

    return 0;
}
