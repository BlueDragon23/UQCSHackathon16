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

#define INIT_ENEMIES 2

// 2-dimensional array of row pin numbers:
const int row[8] = {
  2, 7, 19, 5, 13, 18, 12, 16
};

// 2-dimensional array of column pin numbers:
const int col[8] = {
  6, 11, 10, 3, 17, 4, 8, 9
};

struct Coord {
  int row;
  int col;
};

// player data
struct Coord player = {.row = 0, .col = 4};

int splash = 0;
int tick = 0;
int flash = 0;
int fps = 12;
int tps = 25000;

// enemy data
char direction;
struct Coord positions[INIT_ENEMIES] = {
  {.row = 7, .col = 0},
  {.row = 7, .col = 4}
};
int numEnemies = INIT_ENEMIES;

bool gameOver = false;

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
  direction = 'E';
  delay(1000);
}

void loop() {
  // draw the screen:
  clear_screen();
  
//  int analogX = analogRead(0);
//  int analogY = analogRead(1);
//  if (analogX > 1024*3/4) {
//  } else if (analogX < 1024/4) {
//  } else if (analogY > 1024*3/4) {
//  } else if (analogY < 1024/4) {
//  }
  draw_pixel(0, 0);
  if (tick++ > tps/fps) {
    move_enemy();
    if (gameOver) {
      end_game();
    }
    tick = 0;
  }
  draw_player();
  draw_enemies();
}

void draw_player() {
  draw_pixel(player.row, player.col);
}

void end_game() {
  long gameOverTimer = 0;
  while (gameOverTimer < 100000) {
    if (gameOverTimer % 10000 < 5000) {
      draw_player();
    }
    gameOverTimer++;
  }
  asm volatile(" jmp 0");
}

void move_enemy() {
  if (direction == 'E') {
    if (positions[numEnemies - 1].col == 7) {
      // Move downwards
      for (int i = 0; i < numEnemies; i++) {
        positions[i].row -= 1;
      }
      direction = 'W';
    } else {
      // Move across
      for (int i = 0; i < numEnemies; i++) {
        positions[i].col += 1;
      }
    }
  } else {
    if (positions[0].col == 0) {
      // Move downwards
      for (int i = 0; i < numEnemies; i++) {
        positions[i].row -= 1;
      }
      direction = 'E';
    } else {
      // Move across
      for (int i = 0; i < numEnemies; i++) {
        positions[i].col -= 1;
      }
    }
  }
}

void draw_enemies() {
  for (int i = 0; i < numEnemies; i++) {
    draw_pixel(positions[i].row, positions[i].col);
    draw_pixel(positions[i].row, positions[i].col + 1);
    draw_pixel(positions[i].row + 1, positions[i].col);
    draw_pixel(positions[i].row + 1, positions[i].col + 1);
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


