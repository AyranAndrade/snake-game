#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <ncurses.h>
#include <string.h>

void initializeGame();
void initializeGraphicLibrary();
void initializeRandomSeed();
void initializeGameMatrix();
void finalizeGame();
void finalizeGraphicLibrary();
void finalizeGameMatrix();
void printGame();
void printGameOver();

bool checkIfSnakeCollidedWithBoundaries();
bool checkIfSnakeAteApple();
bool isDirectionValid(char direction);

void drawBoundaries();
void drawApple();
void drawSnakeInitially();

void eraseAllGame();

void moveSnake(char direction);
void moveSnakeHead();
void moveSnakeTail();

void generateAppleLocation();
void generateSnakeLocation();

void sleepInSeconds(float seconds);
char getKeyboardInput();

const int WIDTH = 38;
const int HEIGHT = 23;
int** game;

int snakeHeadI;
int snakeHeadJ;
int snakeTailI;
int snakeTailJ;
bool mustMoveSnakeTail = true;
bool snakeCollidedWithHimself = false;
char snakeLastDirection = 'd';
const float SNAKE_SPEED_IN_SECONDS = 0.1;

const float TIME_AFTER_GAME_OVER_IN_SECONDS = 10;

int appleI;
int appleJ;

int main() {
  initializeGame();
  drawBoundaries();
  generateSnakeLocation();
  drawSnakeInitially();
  generateAppleLocation();

  while (true) {
    char direction = getKeyboardInput();

    drawApple();

    moveSnake(direction);

    mustMoveSnakeTail = true;

    if (snakeCollidedWithHimself) {
      finalizeGame();
      break;
    }

    if (checkIfSnakeAteApple()) {
      generateAppleLocation();
      mustMoveSnakeTail = false;
    }

    if (checkIfSnakeCollidedWithBoundaries()) {
      finalizeGame();
      break;
    }

    printGame();

    sleepInSeconds(SNAKE_SPEED_IN_SECONDS);
  }
}

void initializeGame() {
  initializeGraphicLibrary();
  initializeRandomSeed();
  initializeGameMatrix();
}

void initializeGraphicLibrary() {
  initscr();
  nodelay(stdscr, TRUE);
  noecho();
}

void initializeRandomSeed() {
  srand((unsigned) time(0));
}

void initializeGameMatrix() {
  game = malloc(HEIGHT * sizeof(int*));

  for (int i = 0; i < HEIGHT; i++) {

    game[i] = malloc(WIDTH * sizeof(int));
    for (int j = 0; j < WIDTH; j++) {
      game[i][j] = 0;
    }
  }
}

void generateSnakeLocation() {
  snakeHeadI = rand() % (HEIGHT - 5);
  snakeHeadJ = rand() % (WIDTH - 5);

  if (snakeHeadJ < 5) {
    snakeHeadJ = 5;
  }

  snakeTailI = snakeHeadI;
  snakeTailJ = snakeHeadJ - 4;
}

void drawSnakeInitially() {
  for (int j = snakeHeadJ; j > snakeTailJ; j--) {
    game[snakeHeadI][j] = 2;
  }
}

void finalizeGame() {
  eraseAllGame();
  drawBoundaries();

  printGame();
  printGameOver();

  sleepInSeconds(TIME_AFTER_GAME_OVER_IN_SECONDS);

  finalizeGraphicLibrary();
  finalizeGameMatrix();
}

void finalizeGraphicLibrary() {
  endwin();
}

void finalizeGameMatrix() {
  for (int i = 0; i < HEIGHT; i++) {
    free(game[i]);
  }

  free(game);
}

void drawBoundaries() {
  for (int j = 0; j < WIDTH; j++) {
    game[0][j] = 1;
    game[HEIGHT - 1][j] = 3;
  }

  for (int i = 0; i < HEIGHT; i++) {
    game[i][0] = 1;
    game[i][WIDTH - 1] = 4;
  }
}

void drawApple() {
  game[appleI][appleJ] = 1;
}

void eraseAllGame() {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      game[i][j] = 0;
    }
  }
}

void generateAppleLocation() {
  int i = 0, j = 0;

  while (game[i][j] != 0) {
    i = rand() % HEIGHT;
    j = rand() % WIDTH;

    if (i == 0) {
      i++;
    } else if (i == HEIGHT - 1) {
      i--;
    }

    if (j == 0) {
      j++;
    } else if (j == WIDTH - 1) {
      j--;
    }
  }

  appleI = i;
  appleJ = j;
}

void sleepInSeconds(float seconds) {
  usleep(seconds * 1000000);
}

char getKeyboardInput() {
  return getch();
}

void moveSnake(char direction) {
  if (isDirectionValid(direction)) {
    snakeLastDirection = direction;
  }

  moveSnakeHead();

  if (mustMoveSnakeTail) {
    moveSnakeTail();
  }
}

void moveSnakeHead() {
  if (snakeLastDirection == 'w') {
    snakeHeadI--;
  } else if (snakeLastDirection == 'd') {
    snakeHeadJ++;
  } else if (snakeLastDirection == 's') {
    snakeHeadI++;
  } else if (snakeLastDirection == 'a') {
    snakeHeadJ--;
  } else {
    snakeHeadJ++;
  }

  if (game[snakeHeadI][snakeHeadJ] == 2) {
    snakeCollidedWithHimself = true;
  } else {
    game[snakeHeadI][snakeHeadJ] = 2;
  }
}

void moveSnakeTail() {
  game[snakeTailI][snakeTailJ] = 0;

  if (game[snakeTailI - 1][snakeTailJ] == 2) {
    snakeTailI--;
  } else if (game[snakeTailI + 1][snakeTailJ] == 2) {
    snakeTailI++;
  } else if (game[snakeTailI][snakeTailJ - 1] == 2) {
    snakeTailJ--;
  } else if (game[snakeTailI][snakeTailJ + 1] == 2) {
    snakeTailJ++;
  }
}

bool isDirectionValid(char direction) {
  if (snakeLastDirection == 'w' || snakeLastDirection == 's') {
    if (direction == 'a' || direction == 'd') {
      return true;
    }
  } else if (snakeLastDirection == 'a' || snakeLastDirection == 'd') {
    if (direction == 's' || direction == 'w') {
      return true;
    }
  }

  return false;
}

bool checkIfSnakeCollidedWithBoundaries() {
  if (snakeHeadI == 0) {
    return true;
  } else if (snakeHeadI == HEIGHT - 1) {
    return true;
  } else if (snakeHeadJ == 0) {
    return true;
  } else if (snakeHeadJ == WIDTH - 1) {
    return true;
  }

  return false;
}

bool checkIfSnakeAteApple() {
  if (snakeHeadI == appleI && snakeHeadJ == appleJ) {
    return true;
  }

  return false;
}

void printGameOver() {
  const char* gameOverMessage = "GAME OVER";
  int i = HEIGHT/2;
  int j = WIDTH/2 - strlen(gameOverMessage);
  move(i, j);
  printw("%s", gameOverMessage);
  refresh();
}

void printGame() {
  for (int i = 0; i < HEIGHT; i++) {
    move(i, 0);
    for (int j = 0; j < WIDTH; j++) {
      if (game[i][j] == 0) {
        printw("%s", " ");
      } else {
        printw("%s", "X");
      }
    }
  }

  refresh();
}