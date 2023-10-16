#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <termios.h>
#include <ncurses.h>

void initializeGame();
void initializeGraphicLibrary();
void initializeRandomSeed();
void initializeGameMatrix();
void initializeSnake();
void finalizeGame();
void printGame();
void printUsingGraphicLibrary(const char* message);
void cleanGame();

void drawBoundaries();
void drawApple();
void drawSnake();

void getAppleLocation();
void sleepForOneSecond();
char getKeyboardInput();

int WIDTH = 20;
int HEIGHT = 10;
int** game;

int snakeSize = 4;
int snakeHeadI;
int snakeHeadJ;

int appleI;
int appleJ;

int main() {
  initializeGame();

  // printUsingGraphicLibrary("XXXXXXXXXXXXXXXXX");
  // sleepForOneSecond();
  // sleepForOneSecond();
  // sleepForOneSecond();
  // sleepForOneSecond();
  // sleepForOneSecond();


  while (true) {
    // char abc = getKeyboardInput();
    // if (abc == 'a') {
    //   printw("vc digiotu a");
    // } else if (abc == 'd') {
    //   printw("vc digitou d");
    // }
    drawBoundaries();
    drawSnake();
    getAppleLocation();
    drawApple();
    printGame();
    sleepForOneSecond();
    cleanGame();
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
  game = new int*[HEIGHT];

  for (int i = 0; i < HEIGHT; i++) {

    game[i] = new int[WIDTH];
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
  endwin();
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

void cleanGame() {
  system("clear");

  game[appleI][appleJ] = 0;
}

void getAppleLocation() {
  int i = 0, j = 0;

  while (game[i][j] != 0) {
    i = rand() % HEIGHT;
    j = rand() % WIDTH;

    if (i == 0) {
      i++;
    }

    if (j == 0) {
      j++;
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

void printGame() {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (game[i][j] == 0) {
        printw("%s", " ");
        // printUsingGraphicLibrary(" ");
      } else {
        printw("%s", "X");
        // printUsingGraphicLibrary("X");
      }
    }
  }

  refresh();
}

void printUsingGraphicLibrary(const char* message) {
  // printw("%s", message);
  // refresh();
}