#include "ship.h"
#include "game.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
void createShip(Ship &ship) {
  ship.width = SHIP_BITMAP_WIDTH;
  ship.height = SHIP_BITMAP_HEIGHT;
  ship.coorX = WIDTH / 2.0;
  ship.coorY = HEIGHT / 2.0;
  ship.angle = 90.0;

  ship.acceleration = 0;
  ship.velocityX = 0.0;
  ship.velocityY = 0.0;

  ship.lives = 3;
  ship.score = 0;

  ship.isAlive = true;
  ship.justRevied = true;

  ship.timer = 0;
  ship.isVisible = true;
}

void drawShip() {
  // Because the thinkness of the outline is 3, the 1.5 is so that the lines do
  // not go out of the bitmap
  int size = SHIP_SHAPE.size();
  for (int i = 0; i < size; i++) {
    if (i == size - 1) {
      al_draw_line(SHIP_SHAPE[0].x, SHIP_SHAPE[0].y, SHIP_SHAPE[size - 1].x,
                   SHIP_SHAPE[size - 1].y, al_map_rgb(255, 255, 255), 2.0);
    } else {
      al_draw_line(SHIP_SHAPE[i].x, SHIP_SHAPE[i].y, SHIP_SHAPE[i + 1].x,
                   SHIP_SHAPE[i + 1].y, al_map_rgb(255, 255, 255), 2.0);
    }
  }
}

void rotate_right(Ship &ship) {
  ship.angle -= ROTATE_SPPED;
  if (ship.angle < -180)
    ship.angle = 180;
}
void rotate_left(Ship &ship) {
  ship.angle += ROTATE_SPPED;
  if (ship.angle > 180)
    ship.angle = -180;
}
void move_ship(Ship &ship) {
  // accelerator: reach max speed at 9/60 frame (without friction)
  if (ship.acceleration < 0.15) {
    ship.acceleration += 0.05;
  }
  // Capping the speed
  float speed =
      sqrt(ship.velocityX * ship.velocityX + ship.velocityY * ship.velocityY);
  if (speed < MAX_SPEED) {
    ship.velocityX += ship.acceleration * cos(ship.angle * (M_PI / 180.0));
    ship.velocityY += ship.acceleration * sin(ship.angle * (M_PI / 180.0));
  }
}

void updateShip(Ship &ship) {
  // Apply friction
  ship.velocityX *= 0.991;
  ship.velocityY *= 0.991;

  //  Set ship's position
  ship.coorY += -ship.velocityY;
  ship.coorX += ship.velocityX;

  // Re-position spaceship if it goes out of screen
  if (ship.coorY < 0 - ship.width / 2.0)
    ship.coorY = (HEIGHT + ship.width / 2.0);
  if (ship.coorY > (HEIGHT + ship.width / 2.0))
    ship.coorY = (0 - ship.width / 2.0);

  if (ship.coorX < 0 - ship.width / 2.0)
    ship.coorX = (WIDTH + ship.width / 2.0);
  if (ship.coorX > (WIDTH + ship.width / 2.0))
    ship.coorX = (0 - ship.width / 2.0);

  // In case ship has just been revied
  if (ship.justRevied) {
    ship.timer++;
    if (ship.timer <= 60 * 3) {
      if (ship.timer % 10 == 0) {
        ship.isVisible = !ship.isVisible;
      }
    } else {
      ship.justRevied = false;
      ship.isVisible = true;
      ship.timer = 0;
    }
  }

  if (!ship.isAlive) {
    ship.timer++;
    // Reset ship
    ship.isVisible = false;
    ship.velocityY = 0;
    ship.velocityX = 0;
    ship.coorX = WIDTH / 2.0;
    ship.coorY = HEIGHT / 2.0;
    ship.angle = 90.0;
    // Ship is not showned for 1 second
    if (ship.timer >= 60) {
      ship.timer = 0;
      ship.isAlive = true;
      ship.justRevied = true;
    }
  }
}

Point getVertexLocation(Ship &ship, int vertexNo) {
  float x = (ship.coorX - SHIP_BITMAP_WIDTH / 2.0 + SHIP_SHAPE[vertexNo].x);
  float y = (ship.coorY - SHIP_BITMAP_HEIGHT / 2.0 + SHIP_SHAPE[vertexNo].y);
  float angleRadian = -ship.angle * (M_PI / 180.0);
  float rotatedX = ship.coorX + cos(angleRadian) * (x - ship.coorX) -
                   sin(angleRadian) * (y - ship.coorY);
  float rotatedY = ship.coorY + sin(angleRadian) * (x - ship.coorX) +
                   cos(angleRadian) * (y - ship.coorY);
  return {rotatedX, rotatedY};
}
