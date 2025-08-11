#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define LARGURA 20
#define ALTURA 10

typedef struct Segmento {
    int x, y;
    struct Segmento* proximo;
} Segmento;

enum Direcao { CIMA, BAIXO, ESQUERDA, DIREITA };

// Funções
void inicializarMapa(char mapa[ALTURA][LARGURA]);
void imprimirMapa(char mapa[ALTURA][LARGURA]);
void colocarComida(char mapa[ALTURA][LARGURA], int* comidaX, int* comidaY, Segmento* corpo);
void atualizarMapa(char mapa[ALTURA][LARGURA], Segmento* corpo, int comidaX, int comidaY);
void moverCentopeia(Segmento** cabeca, enum Direcao dir, int* crescer);
int verificarColisao(Segmento* cabeca);
int verificarColisaoCorpo(Segmento* cabeca);

int main() {
    char mapa[ALTURA][LARGURA];
    enum Direcao direcao = DIREITA;
    int fimDeJogo = 0;
    int crescer = 0;
    int comidaX, comidaY;

    srand(time(NULL));

    Segmento* cabeca = (Segmento*)malloc(sizeof(Segmento));
    cabeca->x = LARGURA / 2;
    cabeca->y = ALTURA / 2;
    cabeca->proximo = NULL;

    colocarComida(mapa, &comidaX, &comidaY, cabeca);

    while (!fimDeJogo) {
        if (_kbhit()) {
            char tecla = _getch();
            if (tecla == 'w' && direcao != BAIXO) direcao = CIMA;
            if (tecla == 's' && direcao != CIMA) direcao = BAIXO;
            if (tecla == 'a' && direcao != DIREITA) direcao = ESQUERDA;
            if (tecla == 'd' && direcao != ESQUERDA) direcao = DIREITA;
        }

        moverCentopeia(&cabeca, direcao, &crescer);

        if (verificarColisao(cabeca) || verificarColisaoCorpo(cabeca)) {
            fimDeJogo = 1;
            break;
        }

        if (cabeca->x == comidaX && cabeca->y == comidaY) {
            crescer = 1;
            colocarComida(mapa, &comidaX, &comidaY, cabeca);
        }

        atualizarMapa(mapa, cabeca, comidaX, comidaY);
        system("cls");
        imprimirMapa(mapa);
        _sleep(200);
    }

    printf("Game Over!\n");

    while (cabeca != NULL) {
        Segmento* temp = cabeca;
        cabeca = cabeca->proximo;
        free(temp);
    }

    return 0;
}

void inicializarMapa(char mapa[ALTURA][LARGURA]) {
	int i, j;
    for ( i = 0; i < ALTURA; i++) {
        for ( j = 0; j < LARGURA; j++) {
            if (i == 0 || i == ALTURA - 1 || j == 0 || j == LARGURA - 1) {
                mapa[i][j] = '#';
            } else {
                mapa[i][j] = ' ';
            }
        }
    }
}

void imprimirMapa(char mapa[ALTURA][LARGURA]) {
	int i, j;
    for ( i = 0; i < ALTURA; i++) {
        for ( j = 0; j < LARGURA; j++) {
            printf("%c", mapa[i][j]);
        }
        printf("\n");
    }
}

void colocarComida(char mapa[ALTURA][LARGURA], int* comidaX, int* comidaY, Segmento* corpo) {
    int x, y;
    int ocupado;
    do {
        ocupado = 0;
        x = rand() % (LARGURA - 2) + 1;
        y = rand() % (ALTURA - 2) + 1;

        Segmento* temp = corpo;
        while (temp != NULL) {
            if (temp->x == x && temp->y == y) {
                ocupado = 1;
                break;
            }
            temp = temp->proximo;
        }
    } while (ocupado);

    *comidaX = x;
    *comidaY = y;
}

void atualizarMapa(char mapa[ALTURA][LARGURA], Segmento* corpo, int comidaX, int comidaY) {
    inicializarMapa(mapa);

    mapa[comidaY][comidaX] = '*';

    Segmento* temp = corpo;
    mapa[temp->y][temp->x] = 'O'; // Cabeça
    temp = temp->proximo;
    while (temp != NULL) {
        mapa[temp->y][temp->x] = 'o';
        temp = temp->proximo;
    }
}

void moverCentopeia(Segmento** cabeca, enum Direcao dir, int* crescer) {
    int novoX = (*cabeca)->x;
    int novoY = (*cabeca)->y;

    switch (dir) {
        case CIMA: novoY--; break;
        case BAIXO: novoY++; break;
        case ESQUERDA: novoX--; break;
        case DIREITA: novoX++; break;
    }

    Segmento* novaCabeca = (Segmento*)malloc(sizeof(Segmento));
    novaCabeca->x = novoX;
    novaCabeca->y = novoY;
    novaCabeca->proximo = *cabeca;
    *cabeca = novaCabeca;

    if (!(*crescer)) {
        Segmento* temp = *cabeca;
        while (temp->proximo->proximo != NULL) {
            temp = temp->proximo;
        }
        free(temp->proximo);
        temp->proximo = NULL;
    } else {
        *crescer = 0;
    }
}

int verificarColisao(Segmento* cabeca) {
    return (
        cabeca->x <= 0 || cabeca->x >= LARGURA - 1 ||
        cabeca->y <= 0 || cabeca->y >= ALTURA - 1
    );
}

int verificarColisaoCorpo(Segmento* cabeca) {
    Segmento* temp = cabeca->proximo;
    while (temp != NULL) {
        if (cabeca->x == temp->x && cabeca->y == temp->y) {
            return 1;
        }
        temp = temp->proximo;
    }
    return 0;
}
