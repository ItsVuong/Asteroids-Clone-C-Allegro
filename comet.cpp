#include "comet.h"
#include "game.h"
#include <allegro5/bitmap.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/color.h>
#include <allegro5/display.h>
#include <allegro5/drawing.h>
#include <allegro5/transformations.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
struct point {
  double coorX;
  double coorY;
};
float distanceBetweenPoints(Point a, Point b) {
  float result = sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
  return result;
}
// Initialize comet object
void createComet(Comet &comet, int cometType) {
  comet.coorY = 0.0;
  comet.coorX = 0.0;

  comet.angle = 0.0;
  comet.speed = 0.0;

  comet.isAlive = false;
  comet.bitmap = al_create_bitmap(BITMAP_WIDTH, BITMAP_HEIGHT);
  if (cometType == 2) {
    comet.shape = SHAPES[1];
  } else if (cometType == 3) {
    comet.shape = SHAPES[2];
  } else {
    comet.shape = SHAPES[0];
  }
}
// Draw comet on a bitmap
void drawComet(Comet &comet) {
  // al_clear_to_color(al_map_rgb(115, 115, 115));
  for (int i = 0; i < comet.shape.size() - 1; i++) {
    al_draw_line(comet.shape[i].x, comet.shape[i].y, comet.shape[i + 1].x,
                 comet.shape[i + 1].y, al_map_rgb(255, 255, 255), 0);
  }
  al_draw_line(comet.shape[0].x, comet.shape[0].y, comet.shape[3].x,
               comet.shape[3].y, al_map_rgb(255, 255, 255), 0);
}

// Actually showing the comet and randomizet it's position
void spawnComet(Comet &comet) {
  // Get a random point that is 300 pixel away from the center of the screen
  Point center = newPoint((WIDTH / 2.0), (HEIGHT / 2.0));
  Point cometLocation = newPoint(rand() % WIDTH, rand() % HEIGHT);
  while (distanceBetweenPoints(center, cometLocation) < 300) {
    cometLocation = newPoint(rand() % WIDTH, rand() * HEIGHT);
  }
  comet.isAlive = true;
  comet.coorX = cometLocation.x;
  comet.coorY = cometLocation.y;

  comet.speed = 1;
  comet.angle = rand() % 180 + 1;
  comet.direction = rand() % 180 + 1;
}

void moveComet(Comet comets[], int size) {
  for (int i = 0; i < size; i++) {
    if (comets[i].isAlive) {
      comets[i].coorX +=
          comets[i].speed * cos(comets[i].direction * M_PI / 180.0);
      comets[i].coorY +=
          comets[i].speed * sin(comets[i].direction * M_PI / 180.0);
      comets[i].angle += 0.2;
      if (comets[i].angle > 360)
        comets[i].angle = 0;
      if (comets[i].angle < 0)
        comets[i].angle = 360;

      if (comets[i].coorX > WIDTH + BITMAP_HEIGHT / 2.0) {
        comets[i].coorX = 0 - BITMAP_HEIGHT / 2.0;
      }
      if (comets[i].coorX < 0 - BITMAP_HEIGHT / 2.0) {
        comets[i].coorX = WIDTH + BITMAP_HEIGHT / 2.0;
      }
      if (comets[i].coorY > HEIGHT + BITMAP_HEIGHT / 2.0) {
        comets[i].coorY = 0 - BITMAP_HEIGHT / 2.0;
      }
      if (comets[i].coorY < 0 - BITMAP_HEIGHT / 2.0) {
        comets[i].coorY = HEIGHT + BITMAP_HEIGHT / 2.0;
      }
    }
  }
}

void spawnSmallComet(Comet &parentComet, Comet &smallComet) {
  smallComet.isAlive = true;
  smallComet.coorX = parentComet.coorX;
  smallComet.coorY = parentComet.coorY;

  smallComet.speed = 1 + 0.3 + (rand() % 7) / 10.0;
  smallComet.angle = rand() % 180 + 1;

  int randDirection = rand() % 2 + 1;
  int direction = 1;
  if (randDirection % 2 == 0)
    direction = -1;

  smallComet.direction = parentComet.direction + (rand() % 30 + 1) * direction;
}

Point getVertexLocation(Comet &comet, int i) {
  // Get the actual position of the verices
  //(center of bitmap - half of WIDTH or LENGTH) = starting point of the bitmap
  //(0, 0) From the starting point + Vertices location on respective to the
  // bitmap
  float x = (comet.coorX - BITMAP_WIDTH / 2.0 + comet.shape[i].x);
  float y = (comet.coorY - BITMAP_WIDTH / 2.0 + comet.shape[i].y);
  float angleRadian = comet.angle * (M_PI / 180.0);

  // Apply linear transformation rotation to get the position of the vertices
  float rotatedX = comet.coorX + cos(angleRadian) * (x - comet.coorX) -
                   sin(angleRadian) * (y - comet.coorY);
  float rotatedY = comet.coorY + sin(angleRadian) * (x - comet.coorX) +
                   cos(angleRadian) * (y - comet.coorY);
  return newPoint(rotatedX, rotatedY);
}
