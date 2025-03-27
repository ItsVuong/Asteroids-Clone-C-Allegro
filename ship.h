#ifndef _SHIP_
#define _SHIP_

#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>

#define MAX_SPEED 7
#define ROTATE_SPPED 2.3
struct Ship{
  int width;
  int height;
  double coorX;
  double coorY;
  double angle;
  
  double acceleration;
  double velocityX;
  double velocityY;

  int lives;
  int score;
  int boundX;
  int boundY;
};
void createShip(Ship &ship);
void drawShip(Ship &ship);
void rotate_left(Ship &ship);
void rotate_right(Ship &ship);
void move_ship(Ship &ship);
void friction(Ship &ship);

#endif
