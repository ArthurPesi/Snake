#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define UP 0x57
#define LEFT 0x41
#define DOWN 0x53
#define RIGHT 0x44

struct Snake {
    int position[16][2];
    int head;
    int tail;
    int size;
    int direction[2];
};

struct Game{
    int mapSize;
    char map[8][8];
    int apple[2];
    struct Snake *snake;
};

void renderFrame(struct Game *frame) {
    system("cls");
    int x = 0;
    for(x;x < frame->mapSize; x++) {
        int y = 0;
        for(y; y/2 < frame->mapSize; y++) {
            int drawn = 0;
            for(int i = frame->snake->tail; i <= frame->snake->head;i++) {
                if(frame->snake->position[i][0] == x && frame->snake->position[i][1] == y/2) {
                    if(y % 2 == 0) {
                        printf("[");
                    } else {
                        printf("]");
                    }
                    drawn = 1;
                }
            } 
            if(drawn) {

            } else if(x == frame->apple[0] && y/2 == frame->apple[1]) {
                printf("6");
            } else {
                printf("%c",frame->map[x][y/2]);
            }
        }
        printf("\n");
    }
}

void sortApplePosition(struct Game *game) {
    srand(time(NULL));
    game->apple[0] = (rand() % (game->mapSize - 3)) + 1;
    game->apple[1] = (rand() % (game->mapSize - 3)) + 1;
}

void moveSnake(struct Snake *snake) {
    if(GetAsyncKeyState(UP)) {
        snake->direction[0] = -1;
        snake->direction[1] = 0;
    } else if (GetAsyncKeyState(LEFT)) {
        snake->direction[0] = 0;
        snake->direction[1] = -1;
    } else if (GetAsyncKeyState(DOWN)) {
        snake->direction[0] = 1;
        snake->direction[1] = 0;
    } else if (GetAsyncKeyState(RIGHT)) {
        snake->direction[0] = 0;
        snake->direction[1] = 1;
    }
    int dir[2];
    dir[0] = snake->direction[0] + snake->position[snake->head][0];
    dir[1] = snake->direction[1] + snake->position[snake->head][1];
    int positionSize = sizeof(snake->position)/sizeof(snake->position[0]);
    snake->head = (snake->head + 1) % positionSize;
    snake->tail = (snake->tail + 1) % positionSize;
    snake->position[snake->head][0] = dir[0];
    snake->position[snake->head][1] = dir[1];
}

struct Game *initializeGame() {
    struct Game *newGame = (struct Game*)calloc(sizeof(struct Game),1);
    newGame->snake = (struct Snake*)calloc(sizeof(struct Snake),1);
    newGame->mapSize = sizeof(newGame->map)/sizeof(newGame->map[0]);
    for(int i = 0; i < newGame->mapSize; i++) {
        for(int j = 0; j < newGame->mapSize; j++) {
            if(j == 0 || i == 0 || j == newGame->mapSize -1 || i == newGame->mapSize -1) {
                newGame->map[i][j] = '|';
            } else {
                newGame->map[i][j] = ' ';
            }
        }
    }
    sortApplePosition(newGame);
    newGame->snake->size = 1;
    newGame->snake->head = 0;
    newGame->snake->tail = 0;
    newGame->snake->position[0][0] = (newGame->mapSize-1)/2;
    newGame->snake->position[0][1] = (newGame->mapSize-1)/2;
    newGame->snake->direction[0] = 0;
    newGame->snake->direction[1] = 1;
    return newGame;
}

int gameover(struct Game *currFrame) {
    const struct Snake *snake = currFrame->snake;
    const int x = snake->position[snake->head][0];
    const int y = snake->position[snake->head][1];
    if(currFrame->map[x][y] == '|') {
        return 1;
    } //se a cobra encostar em si mesma
    return 0;
}

int main() {
    struct Game *game = initializeGame();
    renderFrame(game);
    do {
        double frameDuration = 0.5f;
        double timeTaken;
        clock_t start = clock();
        do {
            timeTaken = (double)(clock() - start) / CLOCKS_PER_SEC;
        } while(frameDuration > timeTaken);
        moveSnake(game->snake);
        renderFrame(game);
    } while(!gameover(game));
    free(game->snake);
    free(game);
    return 0;
}