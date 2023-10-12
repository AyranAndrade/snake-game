#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <termios.h>
#include <ncurses.h>
using namespace std;

void initializeGame();
void printGame();
void drawBoundaries();
void drawApple();
void drawSnake();
void cleanGame();
void getAppleLocation();
void sleepForOneSecond();

int WIDTH = 100;
int HEIGHT = 50;
int** game;

int snakeSize = 4;
int snakeHeadI;
int snakeHeadJ;

int appleI;
int appleJ;

int main() {
  srand((unsigned) time(0));

  initializeGame();

  while (true) {
    initscr();
    nodelay(stdscr, TRUE);
    int a = getch();
    cout << a << endl;
    endwin();
    drawBoundaries();
    drawSnake();
    getAppleLocation();
    drawApple();
    printGame();
    sleepForOneSecond();
    cleanGame();
  }
}

void initializeGame() {
  game = new int*[HEIGHT];

  for (int i = 0; i < HEIGHT; i++) {

    game[i] = new int[WIDTH];
    for (int j = 0; j < WIDTH; j++) {
      game[i][j] = 0;
    }
  }

  snakeHeadI = rand() % (HEIGHT - 5);
  snakeHeadJ = rand() % (WIDTH - 5);

  if (snakeHeadJ < 5) {
    snakeHeadJ = 5;
  }
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

void printGame() {
  for (int i = 0; i < HEIGHT; i++) {
    cout << endl;

    for (int j = 0; j < WIDTH; j++) {
      if (game[i][j] == 0) {
        cout << " ";
      } else {
        cout << "X";
      }
    }
  }

  cout << endl;
}