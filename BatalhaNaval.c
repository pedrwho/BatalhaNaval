#include <stdio.h>
#include <stdlib.h>

#define BOARD_SIZE 10

/* Valores no tabuleiro:
   0 -> água
   3 -> navio
   5 -> área afetada por habilidade
*/

/* Gera uma matriz de habilidade do tipo "cone" (tamanho odd, ex: 5x5)
   A origem (topo do cone) é a linha 0 da matriz e o cone "abre" para baixo.
   Para uma matriz de tamanho n (n odd), consideramos o centro da linha como coluna central.
   Regra: na linha i (0..n-1) são afetadas as colunas j cuja distância ao centro <= i.
*/
int **createCone(int n) {
    int center = n / 2;
    int **mat = malloc(n * sizeof(int *));
    for (int i = 0; i < n; ++i) {
        mat[i] = malloc(n * sizeof(int));
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            // Condicional que define a forma do cone: aumenta a largura conforme i (linha)
            if (abs(j - center) <= i) {
                mat[i][j] = 1;
            } else {
                mat[i][j] = 0;
            }
        }
    }
    return mat;
}

/* Gera uma matriz de habilidade em "cruz" (plus). Centro é (center, center).
   Regra: linha central inteira e coluna central inteira são afetadas.
*/
int **createCross(int n) {
    int center = n / 2;
    int **mat = malloc(n * sizeof(int *));
    for (int i = 0; i < n; ++i) {
        mat[i] = malloc(n * sizeof(int));
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == center || j == center) mat[i][j] = 1;
            else mat[i][j] = 0;
        }
    }
    return mat;
}

/* Gera uma matriz que representa a vista frontal de um octaedro (losango/diamante).
   Regra: posição (i,j) afetada se a distância de Manhattan ao centro <= center.
   Isso produz um losango simétrico.
*/
int **createOctahedron(int n) {
    int center = n / 2;
    int **mat = malloc(n * sizeof(int *));
    for (int i = 0; i < n; ++i) {
        mat[i] = malloc(n * sizeof(int));
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int manhattan = abs(i - center) + abs(j - center);
            if (manhattan <= center) mat[i][j] = 1;
            else mat[i][j] = 0;
        }
    }
    return mat;
}

/* Sobrepõe uma matriz de habilidade (mat, size x size) no tabuleiro (board),
   centrando-a em (originRow, originCol) no tabuleiro.
   As células afetadas recebem o valor 5.
   Regras:
     - Calcula deslocamento para alinhar centro da matriz com a origem no tabuleiro.
     - Verifica limites antes de escrever.
     - Usa condicionais e loops aninhados conforme requerido.
*/
void applySkillToBoard(int board[BOARD_SIZE][BOARD_SIZE], int **mat, int size, int originRow, int originCol) {
    int center = size / 2;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (mat[i][j] == 1) {
                int boardRow = originRow - center + i;
                int boardCol = originCol - center + j;

                // Verifica limites do tabuleiro antes de aplicar
                if (boardRow >= 0 && boardRow < BOARD_SIZE && boardCol >= 0 && boardCol < BOARD_SIZE) {
                    // Marca a área afetada com 5 (substitui qualquer valor anterior, como navio 3)
                    board[boardRow][boardCol] = 5;
                }
            }
        }
    }
}

/* Função utilitária para imprimir o tabuleiro com legenda */
void printBoard(int board[BOARD_SIZE][BOARD_SIZE]) {
    printf("Legenda: 0=agua  3=navio  5=area_efeito\n\n");
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

int main() {
    // 1) Cria tabuleiro 10x10 inicializado com 0 (água)
    int board[BOARD_SIZE][BOARD_SIZE];
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            board[r][c] = 0;
        }
    }

    // 2) Coloca alguns navios (valor 3) para visualizar sobreposição
    // Observação: posição fixa no código conforme simplificação do enunciado
    board[2][2] = 3;
    board[2][3] = 3;
    board[2][4] = 3;
    board[5][5] = 3;
    board[7][1] = 3;

    // 3) Define tamanho das matrizes de habilidade (usar tamanho ímpar para ter centro exato)
    int skillSize = 5;

    // 4) Cria matrizes de habilidade dinamicamente usando loops e condicionais
    int **cone = createCone(skillSize);
    int **cross = createCross(skillSize);
    int **octa = createOctahedron(skillSize);

    // 5) Define pontos de origem (linha, coluna) no tabuleiro para cada habilidade
    // Observe: coordenadas baseadas em 0 (0..9)
    int originConeRow = 1, originConeCol = 3;   // cone com origem perto do topo
    int originCrossRow = 4, originCrossCol = 6; // cruz no meio-direita
    int originOctaRow = 6, originOctaCol = 3;   // octaedro/losango no meio-inferior

    // 6) Aplica as habilidades ao tabuleiro (sobreposição)
    applySkillToBoard(board, cone, skillSize, originConeRow, originConeCol);
    applySkillToBoard(board, cross, skillSize, originCrossRow, originCrossCol);
    applySkillToBoard(board, octa, skillSize, originOctaRow, originOctaCol);

    // 7) Exibe o tabuleiro final
    printBoard(board);

    // 8) (Opcional) Exibir as matrizes de habilidade em formato 0/1 para referência
    printf("\nMatriz - Cone (1 = afetado):\n");
    for (int i = 0; i < skillSize; ++i) {
        for (int j = 0; j < skillSize; ++j) {
            printf("%d ", cone[i][j]);
        }
        printf("\n");
    }

    printf("\nMatriz - Cruz (1 = afetado):\n");
    for (int i = 0; i < skillSize; ++i) {
        for (int j = 0; j < skillSize; ++j) {
            printf("%d ", cross[i][j]);
        }
        printf("\n");
    }

    printf("\nMatriz - Octaedro/Losango (1 = afetado):\n");
    for (int i = 0; i < skillSize; ++i) {
        for (int j = 0; j < skillSize; ++j) {
            printf("%d ", octa[i][j]);
        }
        printf("\n");
    }

    // 9) Libera memória alocada
    for (int i = 0; i < skillSize; ++i) {
        free(cone[i]);
        free(cross[i]);
        free(octa[i]);
    }
    free(cone);
    free(cross);
    free(octa);

    return 0;
}
