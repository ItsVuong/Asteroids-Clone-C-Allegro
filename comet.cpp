#include "comet.h"
#include <allegro5/bitmap.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/color.h>
#include <allegro5/display.h>
#include <allegro5/transformations.h>
struct point {
  double coorX;
  double coorY;
};
const point cometShape[] = {{40, 50}, {40, 40}, {50, 40}, {50, 50}};

void drawComet() {
  for (int i = 0; i < 3; i++) {
    al_draw_line(cometShape[i].coorX, cometShape[i].coorY,
                 cometShape[i + 1].coorX, cometShape[i + 1].coorY,
                 al_map_rgb(255, 255, 255), 0);
  }
  al_draw_line(cometShape[0].coorX, cometShape[0].coorY, cometShape[3].coorX,
               cometShape[3].coorY, al_map_rgb(255, 255, 255), 0);
}
