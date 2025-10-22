#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long U64;

U64 SetBit(U64 value, int position) {
    if (position < 0 || position >= 64) return value;
    return value | ((U64)1 << position);
}

U64 ClearBit(U64 value, int position) {
    if (position < 0 || position >= 64) return value;
    return value & ~((U64)1 << position);
}

int GetBit(U64 value, int position) {
    if (position < 0 || position >= 64) return 0;
    return (value >> position) & 1;
}

int CountBits(U64 value) {
    int count = 0;
    while (value) {
        count += value & 1;
        value >>= 1;
    }
    return count;
}


void PrintBoard() {
    for (int row = 7; row >= 0; row--) {
        for (int col = 0; col < 8; col++) {
            int pos = row * 8 + col;
            printf("%3d ", pos);
        }
        printf("\n");
    }
    printf("\n");
}


typedef struct {
    U64 red;
    U64 black;
    U64 kings;
    int turn;
} Game;


void InitGame(Game *g) {
    g->red = 0;
    g->black = 0;
    g->kings = 0;
    g->turn = 1;



    for (int row = 0; row <= 2; row++) {
        for (int col = 0; col < 8; col++) {
            if ((row + col) % 2 == 1) {
                g->red = SetBit(g->red, row * 8 + col);
            }
        }
    }


    for (int row = 5; row <= 7; row++) {
        for (int col = 0; col < 8; col++) {
            if ((row + col) % 2 == 1) {
                g->black = SetBit(g->black, row * 8 + col);
            }
        }
    }
}


void MakeMove(Game *g, int from, int to) {
    U64 *player = g->turn == 0 ? &g->red : &g->black;
    U64 *opponent = g->turn == 0 ? &g->black : &g->red;

    if (!GetBit(*player, from)) {
        printf(" No piece here.\n");
        return;
    }

    if (GetBit(*player, to) || GetBit(*opponent, to)) {
        printf(" Spot is taken\n");
        return;
    }

    int from_row = from / 8;
    int from_col = from % 8;
    int to_row = to / 8;
    int to_col = to % 8;

    int row_diff = to_row - from_row;
    int col_diff = to_col - from_col;


    if (g->turn == 1) {
        if (row_diff != -1 && row_diff != -2) {
            printf(" Black has to move up.\n");
            return;
        }
    } else {
        if (row_diff != 1 && row_diff != 2) {
            printf(" Red has to move down\n");
            return;
        }
    }


    if (abs(col_diff) != abs(row_diff)) {
        printf("Invalid move\n");
        return;
    }


    if (abs(row_diff) == 1) {
        *player = ClearBit(*player, from);
        *player = SetBit(*player, to);



        g->turn = 1 - g->turn;
        return;
    }


    if (abs(row_diff) == 2) {
        int jumped_pos = from + ((to - from) / 2);

        if (!GetBit(*opponent, jumped_pos)) {
            printf("Invalid move\n");
            return;
        }


        *opponent = ClearBit(*opponent, jumped_pos);
        *player = ClearBit(*player, from);
        *player = SetBit(*player, to);

        printf("Captured piece at %d!\n", jumped_pos);



        g->turn = 1 - g->turn;
        return;
    }


    printf("Invalid move.\n");
}

int CheckWin(Game *g) {
    if (CountBits(g->red) == 0) return 1;
    if (CountBits(g->black) == 0) return 2;
    return 0;
}

int main() {
    Game game;
    InitGame(&game);

    while (1) {
        PrintBoard();

        int winner = CheckWin(&game);
        if (winner) {
            printf("Player %s wins!\n", winner == 1 ? "Black" : "Red");
            break;
        }

        printf("Player %s's turn.\n", game.turn == 0 ? "Red" : "Black");

        int from, to;
        printf("Enter move (from to): ");
        scanf("%d %d", &from, &to);

        MakeMove(&game, from, to);
    }

    return 0;
}
