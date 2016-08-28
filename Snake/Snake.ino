/*
 * 
 * Aidan Goldthorpe
 * 
 * LED Matrix Snake
 * 
 * Uses code based on the example below for the matrix display
 ---------

 Pin numbers:
 Matrix:
 * Digital pins 2 through 13,
 * analog pins 2 through 5 used as digital 16 through 19
 Potentiometers:
 * center pins are attached to analog pins 0 and 1, respectively
 * side pins attached to +5V and ground, respectively.

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/RowColumnScanning

 see also http://www.tigoe.net/pcomp/code/category/arduinowiring/514 for more
 */

#define MAX 64

// 2-dimensional array of row pin numbers:
const int row[8] = {
  2, 7, 19, 5, 13, 18, 12, 16
};

// 2-dimensional array of column pin numbers:
const int col[8] = {
  6, 11, 10, 3, 17, 4, 8, 9
};

// 2-dimensional array of pixels:
int pixels[8][8];

const int uqcsPixels[8][8] = {
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 1, 0, 0, 0, 0, 1, 0},
  {0, 0, 1, 0, 0, 1, 0, 0}
};

const int partyParrot[8][8] = {
  {0, 0, 1, 0, 1, 0, 0, 0},
  {0, 1, 1, 1, 1, 1, 0, 0},
  {0, 1, 0, 1, 0, 1, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 0, 1, 1, 1, 0},
  {0, 1, 1, 0, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1}
};

const int partyParrot2[8][8] = {
  {0, 0, 0, 1, 0, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 0, 1, 0, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 0, 1, 1, 1, 0},
  {0, 1, 1, 0, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1}
};

int rachcatch[8][8] = {
  {0, 0, 0, 0, 1, 0, 0, 0},
  {0, 1, 0, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 1, 0},
  {0, 1, 0, 0, 1, 1, 0, 0},
  {0, 1, 0, 0, 0, 1, 1, 0},
  {0, 0, 1, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 0, 0, 0}  
};

int fiestarachcatch[8][8] = {
  {0, 1, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 0, 0, 1, 1, 0, 1},
  {0, 1, 0, 0, 0, 1, 1, 0},
  {0, 0, 1, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 0, 0, 0, 0},
  {0, 0, 1, 1, 1, 0, 0, 0}  
};

struct Coord {
  int row;
  int col;
};

struct Coord board[MAX];

// snek data
int x = 5;
int y = 5;
int snekSize = 0;
char direction = 'N';

int foodR = -1;
int foodC = -1;
int row_ = 0;
int col_ = 0;
int splash = 0;
int fno = 0;
int tick = 0;
int flash = 0;
int fps = 12;
int tps = 25000;

bool gameOver = false;

// queue data
int front = 0;
int rear = -1;
// queue size is snekSize

void setup() {
  // initialize the I/O pins as outputs
  // iterate over the pins:
  for (int thisPin = 0; thisPin < 8; thisPin++) {
    // initialize the output pins:
    pinMode(col[thisPin], OUTPUT);
    pinMode(row[thisPin], OUTPUT);
    // take the col pins (i.e. the cathodes) high to ensure that
    // the LEDS are off:
    digitalWrite(col[thisPin], HIGH);
  }

  // initialize the pixel matrix:
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      pixels[x][y] = HIGH;
    }
  }
  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 8; c++) {
      if (uqcsPixels[r][c]) {
        draw_pixel(r, c);
      }
    }
  }
  push({.row = 5, .col = 5});
  delay(1000);
}

void loop() {
  // draw the screen:
  // refreshScreen();
  clear_screen();
  int analogX = analogRead(0);
  int analogY = analogRead(1);
  if (analogX > 1024*3/4) {
    direction = 'E';
  } else if (analogX < 1024/4) {
    direction = 'W';
  } else if (analogY > 1024*3/4) {
    direction = 'S';
  } else if (analogY < 1024/4) {
    direction = 'N';
  }
  if (tick++ > tps/fps) {
    move();
    if (gameOver) {
      end_game();
    }
    spawn_food();
    tick = 0;
  }
  draw_snek();
  
  if (flash++ < tps/32) {
    draw_pixel(foodR, foodC);
  } else if (flash == tps/16 - 1) {
    flash = 0;
  }
}

void draw_snek() {
  int end_ = rear >= front ? rear : MAX - 1;
  for (int i = front; i <= end_; i++) {
      draw_pixel(board[i].row, board[i].col);
  }
  if (front > rear) {
    for (int i = 0; i <= rear; i++) {
      draw_pixel(board[i].row, board[i].col);
    }
  }
}

void end_game() {
  long gameOverTimer = 0;
  while (gameOverTimer < 100000) {
    if (gameOverTimer % 10000 < 5000) {
      draw_snek();
    }
    gameOverTimer++;
  }
  asm volatile(" jmp 0");
}

void move() {
  if (direction == 'N') {
    y += 1;
  } else if (direction == 'E') {
    x += 1;
  } else if (direction == 'S') {
    y -= 1;
  } else if (direction == 'W') {
    x -= 1;
  }
  // wrap for the moment
  y = y == 8 ? 0 : y;
  x = x == 8 ? 0 : x;
  y = y == -1 ? 7 : y;
  x = x == -1 ? 7 : x;
  
  struct Coord coord = {.row = y, .col = x};
  if (find(coord) != -1) {
    // The snek has collided
    gameOver = true;
    return;
  }
  push(coord);
  if (x == foodC && y == foodR) {
    foodR = -1;
    foodC = -1;
  } else {
    pop();
  }
}

void spawn_food() {
  if (foodR == -1) {
    foodR = random(0, 7);
    foodC = random(0, 7);
  }
}

void clear_screen() {
  for (int r = 0; r < 8; r++) {
    digitalWrite(row[r], LOW);
    digitalWrite(col[r], HIGH);
  }
}

void draw_image(int image[8][8]) {
  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 8; c++) {
      if (image[r][c]) {
        draw_pixel(r, c);
      }
    }
  }
}

void draw_pixel(int r, int c) {
  digitalWrite(row[r], HIGH);
  digitalWrite(col[c], LOW);
  digitalWrite(row[r], LOW);
  digitalWrite(col[c], HIGH);
}

void draw_row(int r) {
  digitalWrite(row[r], HIGH);
  for (int c = 0; c < 8; c++) {
    digitalWrite(col[c], LOW);
    digitalWrite(col[c], HIGH);
  }
  digitalWrite(row[r], LOW);
}

void draw_col(int c) {
  digitalWrite(col[c], LOW);
  for (int i = 0; i < 8; i++) {
    digitalWrite(row[i], HIGH);
    digitalWrite(row[i], LOW);
  }
  digitalWrite(col[c], HIGH);
}

// Queue functions

struct Coord peek() {
  return board[front];
}

bool is_full() {
  return snekSize == MAX;
}

void push(struct Coord coord) {
  if (!is_full()) {
    if (rear == MAX - 1) {
      rear = -1;
    }
    board[++rear] = coord;
    snekSize++;
  }
}

struct Coord pop() {
  struct Coord coord = board[front++];
  if (front == MAX) {
    front = 0;
  }
  snekSize--;
  return coord;
}

int find(struct Coord coord) {
  int end_ = rear >= front ? rear : MAX - 1;
  for (int i = front; i <= end_; i++) {
    if (board[i].row == coord.row && board[i].col == coord.col) {
      return i;
    }
  }
  if (front > rear) {
    for (int i = 0; i <= rear; i++) {
      if (board[i].row == coord.row && board[i].col == coord.col) {
        return i;
      }
    }
  }
  return -1;
}


