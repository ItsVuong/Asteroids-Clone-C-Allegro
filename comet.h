#ifndef _COMET_
#define _COMET_

#include <allegro5/allegro_primitives.h>
#include <allegro5/bitmap.h>
#include <allegro5/color.h>
#include <allegro5/display.h>

const int BITMAP_HEIGHT = 200;
const int BITMAP_WIDTH = 200;

struct Comet {
  double coorX;
  double coorY;
  bool isAlive;
  double speed;
  double angle;

  ALLEGRO_BITMAP *bitmap;
};
void drawComet(Comet &comet);
void createComet(Comet &comet);

#endif
