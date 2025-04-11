#include "ship.h"
#include "game.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
void createShip(Ship &ship) {
  ship.width = 30;
  ship.height = 20;
  ship.coorX = WIDTH / 2.0;
  ship.coorY = HEIGHT / 2.0;
  ship.angle = 0.0;

  ship.acceleration = 0;
  ship.velocityX = 0.0;
  ship.velocityY = 0.0;

  ship.lives = 3;
  ship.score = 0;
  ship.boundX = 6;
  ship.boundY = 7;
}

void drawShip(Ship &ship) {
  // Because the thinkness of the outline is 3, the 1.5 is so that the lines do
  // not go out of the bitmap
  al_draw_triangle(1.5, ship.height - 1.5, 1.5, 0 + 1.5, ship.width,
                   ship.height / 2.0, al_map_rgb(255, 255, 255), 2);
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
}
