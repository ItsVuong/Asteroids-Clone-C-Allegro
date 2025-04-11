#ifndef _COMET_
#define _COMET_

#include "point.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/bitmap.h>
#include <allegro5/color.h>
#include <allegro5/display.h>
#include <vector>

const int BITMAP_HEIGHT = 120;
const int BITMAP_WIDTH = 120;
const std::vector<Point> SHAPES[] = {
    {{10, 10}, {10, 115}, {115, 115}, {115, 10}},
    {{5, 5}, {5, 90}, {90, 90}, {90, 5}},
    {{5, 5}, {5, 40}, {40, 40}, {40, 5}}};
const std::vector<Point> cometShape = {{5, 5}, {5, 90}, {90, 90}, {90, 5}};

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
void createComet(Comet &comet, int cometType);
void spawnComet(Comet &comet);
void moveComet(Comet comets[], int size);

void spawnSmallComet(Comet &parentComet, Comet &smallComet);
Point getVertexLocation(
    Comet &comet,
    int i); // i is the vertex number in the comet's vertices array

#endif
