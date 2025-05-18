#ifndef _COMET_
#define _COMET_

#include "point.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/bitmap.h>
#include <allegro5/color.h>
#include <allegro5/display.h>
#include <vector>

const int COMET_BITMAP_HEIGHT = 120;
const int COMET_BITMAP_WIDTH = 120;
const std::vector<Point> SHAPES[] = {
    {{10, 30}, {10, 50}, {25, 115}, {40, 115},{105, 100},{115, 50},{30, 10}},
    {{5, 5}, {5, 90}, {90, 90}, {90, 5}},
    {{5, 5}, {5, 40}, {40, 40}, {40, 5}}};
    //{{10, 10}, {10, 100}, {115, 115}, {115, 10}},
    //{{5, 5}, {5, 90}, {90, 90}, {90, 5}},
    //{{5, 5}, {5, 40}, {40, 40}, {40, 5}}};
const std::vector<Point> NORMAL_COMET_SHAPE = {
};
const std::vector<Point> SMALL_COMET_SHAPE = {
};
const std::vector<Point> TINY_COMET_SHAPE = {
};
const std::vector<Point> cometShape = {{5, 5}, {5, 90}, {90, 90}, {90, 5}};
typedef enum {
  NORMAL = 0x00,
  SMALL = 0x01,
  TINY = 0x02,
} COMET_TYPE;

struct Comet {
  double coorX;
  double coorY;
  bool isAlive;
  double speed;
  double angle;
  double direction;

  ALLEGRO_BITMAP *bitmap;
  std::vector<Point> shape;
};
void drawComet(Comet &comet);
void createComet(Comet &comet, COMET_TYPE type);
// Comet will spawn far away from a chosen point
void spawnComet(Comet &comet, Point center);
void moveComet(Comet comets[], int size);
void destroyComets(Comet* array, int size);

void spawnSmallComet(Comet &parentComet, Comet &smallComet);
Point getVertexLocation(
    Comet &comet,
    int i); // i is the vertex number in the comet's vertices array

#endif
