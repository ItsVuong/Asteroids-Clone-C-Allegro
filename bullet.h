#ifndef _BULLET_
#define _BULLET_

#include "ship.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>

#define BULLET_SPEED 12.0
#define BULLET_LIFE_TIME 60
#define MAGAZINE_SIZE 20
struct Bullet{
  double coorX;
  double coorY;
  bool isAlive;
  double speed;
  double angle;
  int lifeTime;

  //Velocity of the ship at the time a the bullet is fired
  double velocityX;
  double velocityY;
};

void createBullets(Bullet bullets[]); 
void fireBullet(Bullet bullets[], Ship ship);
void drawBullet(Bullet bullets[], int screendWidth, int screenHeight);
void updateBullets(Bullet bullets[]);

#endif
