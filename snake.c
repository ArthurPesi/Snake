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

int appleCollision(struct Game *game) {
    const int tail = game->snake->tail;
    const int head = game->snake->head;
    const int appleX = game->apple[0];
    const int appleY = game->apple[1];
    for(int i = tail; i <= head; i++) {
        if(appleX == game->snake->position[i][0] && appleY == game->snake->position[i][1]) {
            return 1;
        }
    }
    return 0;
}

void sortApplePosition(struct Game *game) {
    srand(time(NULL)); //nao nascer dentro da cobra
    do {
        game->apple[0] = (rand() % (game->mapSize - 3)) + 1;
        game->apple[1] = (rand() % (game->mapSize - 3)) + 1;
    } while(appleCollision(game));
    game->map[game->apple[0]][game->apple[1]] = '6';
}

int gameover(struct Game *currFrame) {
    const struct Snake *snake = currFrame->snake;
    const int x = snake->position[snake->head][0];
    const int y = snake->position[snake->head][1];
    if(currFrame->map[x][y] == '|' || currFrame->map[x][y] == 'S') {
        return 1;
    } 
    return 0;
}

int comparePreviousState(struct Snake *snake) {
    const int head = snake->head;
    const int prev = head - 1;
    if(head == snake->tail) {
        return 0;
    }
    const int deltaX = snake->position[prev][0] - snake->position[head][0];
    const int deltaY = snake->position[prev][1] - snake->position[head][1];
    if(deltaX == -1 && deltaY == 0) {
        return UP;
    }
    if(deltaX == 0 && deltaY == -1) {
        return LEFT;
    }
    if(deltaX == 1 && deltaY == 0) {
        return DOWN;
    }
    if(deltaX == 0 && deltaY == 1) {
        return RIGHT;
    }
}

int moveSnake(struct Game *frame) {
    struct Snake *snake = frame->snake;

    int forbiddenDir = comparePreviousState(snake);
    if(GetAsyncKeyState(UP) && forbiddenDir != UP) {
        snake->direction[0] = -1;
        snake->direction[1] = 0;
    } else if (GetAsyncKeyState(LEFT) && forbiddenDir != LEFT) {
        snake->direction[0] = 0;
        snake->direction[1] = -1;
    } else if (GetAsyncKeyState(DOWN) && forbiddenDir != DOWN) {
        snake->direction[0] = 1;
        snake->direction[1] = 0;
    } else if (GetAsyncKeyState(RIGHT) && forbiddenDir != RIGHT) {
        snake->direction[0] = 0;
        snake->direction[1] = 1;
    }
    
    int headX = snake->position[snake->head][0];
    int headY = snake->position[snake->head][1];
    int appleX = frame->apple[0];
    int appleY = frame->apple[1];

    if(headX == appleX && headY == appleY) {
        sortApplePosition(frame);
    } else {
        int tailX = snake->position[snake->tail][0];
        int tailY = snake->position[snake->tail][1];
        frame->map[tailX][tailY] = ' ';
        snake->tail = (snake->tail + 1) % snake->positionSize;
    }
    
    int dir[2]; 
    dir[0] = snake->direction[0] + snake->position[snake->head][0];
    dir[1] = snake->direction[1] + snake->position[snake->head][1];

    snake->head = (snake->head + 1) % snake->positionSize;
    snake->position[snake->head][0] = dir[0];
    snake->position[snake->head][1] = dir[1];
    int gameOver = gameover(frame);

    frame->map[dir[0]][dir[1]] = 'S';
    
    return gameOver;
}

struct Game *initializeGame() {
    struct Game *newGame = (struct Game*)calloc(sizeof(struct Game),1);
    newGame->mapSize = sizeof(newGame->map)/sizeof(newGame->map[0]);
    int snakeMaxSize = newGame->mapSize * newGame->mapSize;
    newGame->snake = (struct Snake*)calloc(sizeof(struct Snake) + snakeMaxSize * sizeof(int) * 2,1);
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
    double frameDuration = 0.42f;
    double timeTaken;
    clock_t start = clock();
    do {
        timeTaken = (double)(clock() - start) / CLOCKS_PER_SEC;
    } while(frameDuration > timeTaken);
}

int main() {
    struct Game *game = initializeGame();
    renderFrame(game);
    while(1) {
        delayFrame();
        if(moveSnake(game)) {
            break;
        }
        renderFrame(game);
    }
    free(game->snake);
    free(game);
    return 0;
}