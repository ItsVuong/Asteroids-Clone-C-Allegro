#ifndef _COMET_
#define _COMET_

#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <allegro5/display.h>

struct Comet {
  double coorX;
  double coorY;
  bool isAlive;
  double speed;
  double angle;
};
void drawComet();

#endif
