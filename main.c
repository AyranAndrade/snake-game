#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <ncurses.h>

void initializeGame();
void initializeGraphicLibrary();
void initializeRandomSeed();
void initializeGameMatrix();
void initializeSnake();
void finalizeGame();
void finalizeGraphicLibrary();
void finalizeGameMatrix();
void printGame();

void drawBoundaries();
void drawApple();
void drawSnake();

void eraseApple();
void eraseSnake();

void moveSnake(char direction);

void generateAppleLocation();
void sleepForOneSecond();
char getKeyboardInput();

int WIDTH = 100;
int HEIGHT = 50;
int** game;

int snakeSize = 4;
int snakeHeadI;
int snakeHeadJ;

int appleI;
int appleJ;

int main() {
  initializeGame();
  drawBoundaries();

  while (true) {
    // char abc = getKeyboardInput();
    // if (abc == 'a') {
    //   printw("vc digiotu a");
    // } else if (abc == 'd') {
    //   printw("vc digitou d");
    // }
    moveSnake('d');
    drawSnake();
    generateAppleLocation();
    drawApple();
    printGame();
    sleepForOneSecond();
    eraseApple();
    eraseSnake();
  }

  finalizeGame();
}

void initializeGame() {
  initializeGraphicLibrary();
  initializeRandomSeed();
  initializeGameMatrix();
  initializeSnake();
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

void initializeSnake() {
  snakeHeadI = rand() % (HEIGHT - 5);
  snakeHeadJ = rand() % (WIDTH - 5);

  if (snakeHeadJ < 5) {
    snakeHeadJ = 5;
  }
}

void finalizeGame() {
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
    game[HEIGHT - 1][j] = 1;
  }

  for (int i = 0; i < HEIGHT; i++) {
    game[i][0] = 1;
    game[i][WIDTH - 1] = 1;
  }
}

void drawApple() {
  game[appleI][appleJ] = 1;
}

void drawSnake() {
  for (int b = snakeHeadJ; b > snakeHeadJ - snakeSize; b--) {
    game[snakeHeadI][b] = 1;
  }
}

void eraseApple() {
  game[appleI][appleJ] = 0;
}

void eraseSnake() {
  for (int b = snakeHeadJ; b > snakeHeadJ - snakeSize; b--) {
    game[snakeHeadI][b] = 0;
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

void sleepForOneSecond() {
  usleep(1000000);
}

char getKeyboardInput() {
  return getch();
}

void moveSnake(char direction) {
  if (direction == 'w') {
    snakeHeadI--;
  } else if (direction == 'd') {
    snakeHeadJ++;
  } else if (direction == 's') {
    snakeHeadI++;
  } else if (direction == 'a') {
    snakeHeadJ--;
  } else {
    snakeHeadJ++;
  }
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