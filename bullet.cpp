#include "bullet.h"
#include "main.h"

void createBullets(Bullet bullets[]) {
  for (int i = 0; i < MAGAZINE_SIZE; i++) {
    bullets[i].isAlive = false;
    bullets[i].speed = BULLET_SPEED;
  }
}

void fireBullet(Bullet bullets[], Ship ship) {
  for (int i = 0; i < MAGAZINE_SIZE; i++) {
    if (!bullets[i].isAlive) {
      bullets[i].isAlive = true;
      bullets[i].angle = ship.angle;
      bullets[i].coorX = ship.coorX;
      bullets[i].coorY = ship.coorY;
      bullets[i].velocityX = ship.velocityX;
      bullets[i].velocityY = ship.velocityY;
      bullets[i].lifeTime = 0;
      break;
    }
  }
}

void drawBullet(Bullet bullets[], int screendWidth, int screenHeight) {
  for (int i = 0; i < MAGAZINE_SIZE; i++) {
    if (bullets[i].isAlive) {
      al_draw_circle(bullets[i].coorX, bullets[i].coorY, 1,
                     al_map_rgb(255, 255, 255), 1.5);
    }
  }
}

void updateBullets(Bullet bullets[]) {
  for (int i = 0; i < MAGAZINE_SIZE; i++) {
    if (bullets[i].isAlive) {
      // The foward velocity of the bullet + the velocity of the ship at the
      // time the bullet is fired
      bullets[i].coorX +=
          bullets[i].speed * cos(bullets[i].angle * (M_PI / 180.0)) +
          bullets[i].velocityX * 0.5;
      bullets[i].coorY -=
          bullets[i].speed * sin(bullets[i].angle * (M_PI / 180.0)) +
          bullets[i].velocityY * 0.5;
      // Increase the time that the bullet has lived each iteration
      bullets[i].lifeTime += 1;
    }
    // Reposition the bullet at the other side of the screen if it beyond the
    // screen's edges
    if (bullets[i].coorX > WIDTH)
      bullets[i].coorX = 0;
    if (bullets[i].coorX < 0)
      bullets[i].coorX = WIDTH;
    if (bullets[i].coorY > HEIGHT)
      bullets[i].coorY = 0;
    if (bullets[i].coorY < 0)
      bullets[i].coorY = HEIGHT;
    // Expire bullet
    if (bullets[i].lifeTime >= BULLET_LIFE_TIME) {
      bullets[i].isAlive = false;
      bullets[i].lifeTime = 0;
    }
  }
}
