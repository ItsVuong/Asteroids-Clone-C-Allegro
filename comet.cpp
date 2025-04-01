#include "comet.h"
#include "asteroids.h"
#include <allegro5/bitmap.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/color.h>
#include <allegro5/display.h>
#include <allegro5/drawing.h>
#include <allegro5/transformations.h>
#include <cmath>
struct point {
  double coorX;
  double coorY;
};
const point cometShape[] = {{10, 10}, {10, 190}, {190, 190}, {190, 10}};

void createComet(Comet &comet){
  comet.coorY = 0.0;
  comet.coorX = 0.0;

  comet.angle = 0.0;
  comet.speed = 0.0;

  comet.isAlive = false;
  comet.bitmap = al_create_bitmap(100, 100);
}

void drawComet(Comet &comet) {
  al_clear_to_color(al_map_rgb(115, 115, 115));
  for (int i = 0; i < 3; i++) {
    al_draw_line(cometShape[i].coorX, cometShape[i].coorY,
                 cometShape[i + 1].coorX, cometShape[i + 1].coorY,
                 al_map_rgb(255, 255, 255), 0);
  }
  al_draw_line(cometShape[0].coorX, cometShape[0].coorY, cometShape[3].coorX,
               cometShape[3].coorY, al_map_rgb(255, 255, 255), 0);
}

void spawnComet(Comet &comet){
  comet.isAlive = true;
  comet.coorX = WIDTH;
  comet.coorY = HEIGHT;

  comet.speed = 1;
  comet.angle = 0.0;
}

void moveComet(Comet &comet) {
  comet.coorX += comet.speed * cos(M_PI / 180.0);
  comet.coorY += comet.speed * sin(M_PI / 180.0);

  if(comet.coorX > WIDTH){
    comet.coorX = 0;
  }
  if(comet.coorX < WIDTH){
    comet.coorX = WIDTH;
  }
  if(comet.coorY > HEIGHT){
    comet.coorY = 0;
  }
  if(comet.coorY < HEIGHT){
    comet.coorY = HEIGHT;
  }
}
