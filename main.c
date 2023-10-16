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

void drawBoundaries();
void drawApple();
void drawSnake();
void redrawErasedBoundary();

void eraseApple();
void eraseSnake();

void moveSnake(char direction);

void generateAppleLocation();
void generateSnakeLocation();

void sleepInSeconds(float seconds);
char getKeyboardInput();

const int WIDTH = 100;
const int HEIGHT = 50;
int** game;

int snakeSize = 4;
int snakeHeadI;
int snakeHeadJ;
const float SNAKE_SPEED_IN_SECONDS = 0.1;
const int HOW_MUCH_SNAKE_INCREASE = 2;

const float TIME_AFTER_GAME_OVER_IN_SECONDS = 10;

int appleI;
int appleJ;

int main() {
  initializeGame();
  drawBoundaries();
  generateSnakeLocation();
  generateAppleLocation();

  while (true) {
    // char abc = getKeyboardInput();
    // if (abc == 'a') {
    //   printw("vc digiotu a");
    // } else if (abc == 'd') {
    //   printw("vc digitou d");
    // }
    moveSnake('d');

    if (checkIfSnakeCollidedWithBoundaries()) {
      finalizeGame();
      break;
    }

    if (checkIfSnakeAteApple()) {
      eraseApple();
      generateAppleLocation();
      snakeSize += HOW_MUCH_SNAKE_INCREASE;
    }

    drawSnake();
    drawApple();

    printGame();

    sleepInSeconds(SNAKE_SPEED_IN_SECONDS);

    eraseSnake();
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
}

void finalizeGame() {
  eraseApple();
  eraseSnake();

  redrawErasedBoundary();
  printGame();
  printGameOver();

  sleepInSeconds(TIME_AFTER_GAME_OVER_IN_SECONDS);

  finalizeGraphicLibrary();
  finalizeGameMatrix();
}

void redrawErasedBoundary() {
  game[snakeHeadI][snakeHeadJ] = 1;
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

void sleepInSeconds(float seconds) {
  usleep(seconds * 1000000);
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

bool checkIfSnakeCollidedWithBoundaries() {
  int snakeTailJ = snakeHeadJ - snakeSize;

  if (snakeHeadI == 0) {
    return true;
  } else if (snakeHeadI == HEIGHT - 1) {
    return true;
  } else if (snakeHeadJ == 0) {
    return true;
  } else if (snakeHeadJ == WIDTH - 1) {
    return true;
  } else if (snakeTailJ == 0) {
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