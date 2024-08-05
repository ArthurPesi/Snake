#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define UP 0x57
#define LEFT 0x41
#define DOWN 0x53
#define RIGHT 0x44

struct Snake {
    int head;
    int tail;
    int size;
    int direction[2];
    int positionSize;
    int position[][2];
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
            if(frame->map[x][y/2] == 'S') {
                 if(y % 2 == 0) {
                    printf("[");
                } else {
                    printf("]");
                }
            } else {
                printf("%c",frame->map[x][y/2]);
            }
        }
        printf("\n");
    }
}

void sortApplePosition(struct Game *game) {
    srand(time(NULL)); //nao nascer dentro da cobra
    game->apple[0] = (rand() % (game->mapSize - 3)) + 1;
    game->apple[1] = (rand() % (game->mapSize - 3)) + 1;
    game->map[game->apple[0]][game->apple[1]] = '6';
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

int moveSnake(struct Game *frame) {
    struct Snake *snake = frame->snake;
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
    int tailX = snake->position[snake->tail][0];
    int tailY = snake->position[snake->tail][1];
    frame->map[tailX][tailY] = ' ';
    int dir[2];
    dir[0] = snake->direction[0] + snake->position[snake->head][0];
    dir[1] = snake->direction[1] + snake->position[snake->head][1];
    snake->head = (snake->head + 1) % snake->positionSize;
    snake->tail = (snake->tail + 1) % snake->positionSize;
    snake->position[snake->head][0] = dir[0];
    snake->position[snake->head][1] = dir[1];
    int gameOver = gameover(frame);
    frame->map[dir[0]][dir[1]] = 'S';
    return gameOver;
}

struct Game *initializeGame() {
    struct Game *newGame = (struct Game*)calloc(sizeof(struct Game),1);
    newGame->mapSize = sizeof(newGame->map)/sizeof(newGame->map[0]);
    int snakeMaxSize = newGame->mapSize * newGame->mapSize * 2;
    newGame->snake = (struct Snake*)calloc(sizeof(struct Snake) + snakeMaxSize,1);
    newGame->snake->positionSize = snakeMaxSize;
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
    int initialPosition = (newGame->mapSize-1)/2;
    newGame->snake->position[0][0] = initialPosition;
    newGame->snake->position[0][1] = initialPosition;
    newGame->map[initialPosition][initialPosition] = 'S';
    newGame->snake->direction[0] = 0;
    newGame->snake->direction[1] = 1;
    return newGame;
}

void delayFrame() {
    double frameDuration = 0.5f;
    double timeTaken;
    clock_t start = clock();
    do {
        timeTaken = (double)(clock() - start) / CLOCKS_PER_SEC;
    } while(frameDuration > timeTaken);
}

int main() {
    struct Game *game = initializeGame();
    while(1) {
        renderFrame(game);
        delayFrame();
        if(moveSnake(game)) {
            break;
        }
    }
    free(game->snake);
    free(game);
    return 0;
}