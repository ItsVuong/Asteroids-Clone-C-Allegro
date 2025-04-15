#ifndef _SHIP_
#define _SHIP_

#include "comet.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <vector>

#define MAX_SPEED 6
#define ROTATE_SPPED 3.0
#define SHIP_BITMAP_WIDTH 35
#define SHIP_BITMAP_HEIGHT 28
const std::vector<Point> SHIP_SHAPE = {
    {1.5, SHIP_BITMAP_HEIGHT - 1.5},
    {SHIP_BITMAP_WIDTH - 23.0, SHIP_BITMAP_HEIGHT / 2.0},
    {1.5, 1.5},
    {SHIP_BITMAP_WIDTH - 2.0, SHIP_BITMAP_HEIGHT / 2.0}
};

struct Ship {
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
  bool isAlive;
  bool justRevied;

  int timer;
  bool isVisible;
};
void createShip(Ship &ship);
void drawShip();
void rotate_left(Ship &ship);
void rotate_right(Ship &ship);
void move_ship(Ship &ship);
void updateShip(Ship &ship);
Point getVertexLocation(Ship &ship, int vertextNo);

#endif
