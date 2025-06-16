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

// Direções
enum Direcao { CIMA, BAIXO, ESQUERDA, DIREITA };

// Funções
void inicializarMapa(char mapa[ALTURA][LARGURA]);
void imprimirMapa(char mapa[ALTURA][LARGURA]);
void colocarComida(char mapa[ALTURA][LARGURA]);
int verificarColisao(Segmento* cabeca, char mapa[ALTURA][LARGURA]);
void moverCentopeia(Segmento** cabeca, enum Direcao dir, int* crescer);
int verificarColisaoCorpo(Segmento* cabeca);

int main() {
    char mapa[ALTURA][LARGURA];
    enum Direcao direcao = DIREITA;
    int fimDeJogo = 0;
    int crescer = 0;

    srand(time(NULL));

    Segmento* cabeca = (Segmento*)malloc(sizeof(Segmento));
    cabeca->x = LARGURA / 2;
    cabeca->y = ALTURA / 2;
    cabeca->proximo = NULL;

    inicializarMapa(mapa);
    colocarComida(mapa);

    while (!fimDeJogo) {
        if (_kbhit()) {
            char tecla = _getch();
            if (tecla == 'w' && direcao != BAIXO) direcao = CIMA;
            if (tecla == 's' && direcao != CIMA) direcao = BAIXO;
            if (tecla == 'a' && direcao != DIREITA) direcao = ESQUERDA;
            if (tecla == 'd' && direcao != ESQUERDA) direcao = DIREITA;
        }

        moverCentopeia(&cabeca, direcao, &crescer);

        if (verificarColisao(cabeca, mapa) || verificarColisaoCorpo(cabeca)) {
            fimDeJogo = 1;
            break;
        }

        if (mapa[cabeca->y][cabeca->x] == '*') {
            crescer = 1;
            mapa[cabeca->y][cabeca->x] = ' ';
            colocarComida(mapa);
        }

        inicializarMapa(mapa);
        colocarComida(mapa);

        Segmento* temp = cabeca;
        mapa[temp->y][temp->x] = 'O';
        temp = temp->proximo;
        while (temp != NULL) {
            mapa[temp->y][temp->x] = 'o';
            temp = temp->proximo;
        }

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
    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < LARGURA; j++) {
            if (i == 0 || i == ALTURA - 1 || j == 0 || j == LARGURA - 1) {
                mapa[i][j] = '#';
            } else {
                mapa[i][j] = ' ';
            }
        }
    }
}

void imprimirMapa(char mapa[ALTURA][LARGURA]) {
    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < LARGURA; j++) {
            printf("%c", mapa[i][j]);
        }
        printf("\n");
    }
}

void colocarComida(char mapa[ALTURA][LARGURA]) {
    int x, y;
    do {
        x = rand() % (LARGURA - 2) + 1;
        y = rand() % (ALTURA - 2) + 1;
    } while (mapa[y][x] != ' ');
    mapa[y][x] = '*';
}

void moverCentopeia(Segmento** cabeca, enum Direcao dir, int* crescer) {
    int novoX = (*cabeca)->x;
    int novoY = (*cabeca)->y;

    switch (dir) {
        case CIMA:    novoY--; break;
        case BAIXO:   novoY++; break;
        case ESQUERDA:novoX--; break;
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

int verificarColisao(Segmento* cabeca, char mapa[ALTURA][LARGURA]) {
    if (cabeca->x <= 0 || cabeca->x >= LARGURA - 1 || cabeca->y <= 0 || cabeca->y >= ALTURA - 1) {
        return 1;
    }
    if (mapa[cabeca->y][cabeca->x] == '#') {
        return 1;
    }
    return 0;
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

