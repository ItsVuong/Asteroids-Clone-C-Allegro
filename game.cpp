#include "game.h"
#include "bullet.h"
#include "collision.h"
#include "comet.h"
#include "main.h"
#include "ship.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/bitmap.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/color.h>
#include <allegro5/display.h>
#include <allegro5/drawing.h>
#include <allegro5/events.h>
#include <allegro5/keyboard.h>
#include <allegro5/keycodes.h>
#include <allegro5/system.h>
#include <allegro5/timer.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

const int COMET_SIZE = 9;
void onNormalCometCollide(Comet &comet, Comet smallComets[], int size);
void onSmallCometCollide(Comet &parentComet, Comet tinyComets[], int size);
void onTinyCometCollide(Comet &parentComet, Comet tinyComets[], int size);
void onShipCollide(Ship &ship);

void checkForCollision(Comet cometArray[], Bullet bullets[],
                       int parentCometsSize,
                       void (*collisionHandler)(Comet &, Comet[], int),
                       Comet childComets[], int childCometsSize);

void checkForShipCollision(Ship &ship, Comet cometArray[], int cometArraySize,
                           void (*ccollisionHandler)(Ship &ship));

Ship ship;
// Objects
Bullet bullets[MAGAZINE_SIZE];
Comet cometArray[COMET_SIZE];
Comet smallCometArray[COMET_SIZE * 2];
Comet tinyCometArray[COMET_SIZE * 2 * 2];
// Track if all comets has been destroyed
int activeComets = 0;

// Start game
int game(ALLEGRO_DISPLAY *display, ALLEGRO_TIMER *timer,
         ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font) {
  ALLEGRO_FONT *titleFont =
      al_load_ttf_font("assets/ASTEROID TYPE demo.ttf", 220, 0);
  ALLEGRO_FONT *menuFont = al_load_ttf_font("assets/DePixelKlein.ttf", 20, 0);
  bool redraw = true;
  bool keys[5] = {false, false, false, false, false};
  srand(time(NULL));

  // Create objects
  createShip(ship);
  ALLEGRO_BITMAP *ship_image = al_create_bitmap(ship.width, ship.height);
  al_set_target_bitmap(ship_image);
  al_clear_to_color(
      al_map_rgba(0, 0, 0, 0)); // Clear and make the bitmap transparent
  drawShip();                   // Draw ship on the bitmap's dimension
  al_set_target_bitmap(al_get_backbuffer(display));
  createBullets(bullets);

  // Initialize normal size comets
  for (int i = 0; i < COMET_SIZE; i++) {
    createComet(cometArray[i], NORMAL);
    spawnComet(cometArray[i], {(float)ship.coorX, (float)ship.coorY});
    al_set_target_bitmap(cometArray[i].bitmap);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    drawComet(cometArray[i]);
  }
  // Initalize small comets
  for (int i = 0; i < COMET_SIZE * 2; i++) {
    createComet(smallCometArray[i], SMALL);
    al_set_target_bitmap(smallCometArray[i].bitmap);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    drawComet(smallCometArray[i]);
  }
  for (int i = 0; i < COMET_SIZE * 2 * 2; i++) {
    createComet(tinyCometArray[i], TINY);
    al_set_target_bitmap(tinyCometArray[i].bitmap);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    drawComet(tinyCometArray[i]);
  }
  al_set_target_bitmap(al_get_backbuffer(display));

  while (ship.lives > 0) {
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);
    // Read keyboard events
    if (event.type == ALLEGRO_EVENT_KEY_DOWN ||
        event.type == ALLEGRO_EVENT_KEY_UP) {
      bool key_state = (event.type == ALLEGRO_EVENT_KEY_DOWN);
      switch (event.keyboard.keycode) {
      case ALLEGRO_KEY_UP:
        keys[UP] = key_state;
        break;
      case ALLEGRO_KEY_LEFT:
        keys[LEFT] = key_state;
        break;
      case ALLEGRO_KEY_RIGHT:
        keys[RIGHT] = key_state;
        break;
      case ALLEGRO_KEY_SPACE:
        keys[SPACE] = key_state;
        break;
      }
    }

    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      ship.lives = 0;
    } else if (event.type == ALLEGRO_EVENT_TIMER) {
      if (keys[UP]) {
        move_ship(ship);
      }
      if (!keys[UP]) {
        ship.acceleration = 0;
      }
      if (keys[LEFT]) {
        rotate_left(ship);
      }
      if (keys[RIGHT]) {
        rotate_right(ship);
      }
      if (keys[SPACE]) {
        if (ship.isAlive) {
          fireBullet(bullets, ship);
        }
        keys[SPACE] = false;
      }
      updateBullets(bullets);
      updateShip(ship);
      moveComet(cometArray, COMET_SIZE);
      moveComet(smallCometArray, COMET_SIZE * 2);
      moveComet(tinyCometArray, COMET_SIZE * 2 * 2);
      // Check for bullet and comet collisions
      checkForCollision(cometArray, bullets, COMET_SIZE, onNormalCometCollide,
                        smallCometArray, COMET_SIZE * 2);

      checkForCollision(smallCometArray, bullets, COMET_SIZE * 2,
                        onSmallCometCollide, tinyCometArray,
                        COMET_SIZE * 2 * 2);
      checkForCollision(tinyCometArray, bullets, COMET_SIZE * 2 * 2,
                        onTinyCometCollide, tinyCometArray, COMET_SIZE * 2 * 2);
      // Check for ship collision
      // 3 separate if to prevent the ship can collide with multiple comets at
      // the same time
      if (!ship.justRevied && ship.isAlive) {
        checkForShipCollision(ship, cometArray, COMET_SIZE, onShipCollide);
      }
      if (!ship.justRevied && ship.isAlive) {
        checkForShipCollision(ship, smallCometArray, COMET_SIZE * 2,
                              onShipCollide);
      }
      if (!ship.justRevied && ship.isAlive) {
        checkForShipCollision(ship, tinyCometArray, COMET_SIZE * 2 * 2,
                              onShipCollide);
      }

      // Check if all comets has been destroyed
      activeComets = 0;
      for (int i = 0; i < COMET_SIZE * 2 * 2; i++) {
        if (cometArray[i].isAlive && i < COMET_SIZE) {
          activeComets++;
        }
        if (smallCometArray[i].isAlive && i < (COMET_SIZE * 2)) {
          activeComets++;
        }
        if (tinyCometArray[i].isAlive && i < (COMET_SIZE * 2 * 2)) {
          activeComets++;
        }
      }
      if (activeComets == 0) {
        for (int i = 0; i < COMET_SIZE; i++) {
          spawnComet(cometArray[i], {(float)ship.coorX, (float)ship.coorY});
        }
      }

      redraw = true;
    }

    // Only render every 1/60 second and when there are no event
    if (redraw && al_is_event_queue_empty(event_queue)) {
      al_clear_to_color(al_map_rgb(0, 0, 0));
      redraw = false;
      drawBullet(bullets, WIDTH, HEIGHT);

      // Draw comet
      for (int i = 0; i < COMET_SIZE; i++) {
        if (cometArray[i].isAlive) {
          al_draw_rotated_bitmap(cometArray[i].bitmap, COMET_BITMAP_WIDTH / 2.0,
                                 COMET_BITMAP_HEIGHT / 2.0, cometArray[i].coorX,
                                 cometArray[i].coorY,
                                 cometArray[i].angle * (M_PI / 180.0), 0);
        }
      }
      for (int i = 0; i < COMET_SIZE * 2; i++) {
        if (smallCometArray[i].isAlive) {
          al_draw_rotated_bitmap(
              smallCometArray[i].bitmap, COMET_BITMAP_WIDTH / 2.0,
              COMET_BITMAP_HEIGHT / 2.0, smallCometArray[i].coorX,
              smallCometArray[i].coorY,
              smallCometArray[i].angle * (M_PI / 180.0), 0);
        }
      }
      for (int i = 0; i < COMET_SIZE * 2 * 2; i++) {
        if (tinyCometArray[i].isAlive) {
          al_draw_rotated_bitmap(
              tinyCometArray[i].bitmap, COMET_BITMAP_WIDTH / 2.0,
              COMET_BITMAP_HEIGHT / 2.0, tinyCometArray[i].coorX,
              tinyCometArray[i].coorY, tinyCometArray[i].angle * (M_PI / 180.0),
              0);
        }
      }
      // Draw ship
      if (ship.isVisible) {
        al_draw_rotated_bitmap(ship_image, ship.width / 2.0, ship.height / 2.0,
                               ship.coorX, ship.coorY,
                               -ship.angle * (M_PI / 180.0), 0);
      }

      // Log to screen
      al_draw_textf(font, al_map_rgb(255, 255, 255), 20, 20, 0, "SCORE: %d",
                    ship.score);
      for (int i = 0; i < ship.lives; i++) {
        al_draw_rotated_bitmap(ship_image, ship.width / 2.0, ship.height / 2.0,
                               WIDTH - 40 - i * 30, 30, -90 * (M_PI / 180.0),
                               0);
      }
      al_flip_display();
    }
  }

  while (true) {
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);
    // Read keyboard events
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
      break;
    }
    if (ship.lives == 0) {
      al_clear_to_color(al_map_rgb(0, 0, 0));
      al_draw_textf(titleFont, al_map_rgb(255, 255, 255), WIDTH / 2.0, 100,
                    ALLEGRO_ALIGN_CENTER, "GAME OVER");
      al_draw_textf(menuFont, al_map_rgb(255, 255, 255), WIDTH / 2.0, 500,
                    ALLEGRO_ALIGN_CENTER, "YOUR SCORE: %d", ship.score);
      al_flip_display();
    }
  }

  // Cleanup
  al_destroy_bitmap(ship_image);
  destroyComets(cometArray, COMET_SIZE);
  destroyComets(smallCometArray, COMET_SIZE * 2);
  destroyComets(tinyCometArray, COMET_SIZE * 4);
  al_destroy_font(menuFont);
  al_destroy_font(titleFont);

  return 0;
}

// Handle the event of a bullet hitting a comet
void onNormalCometCollide(Comet &parentComet, Comet smallComets[], int size) {
  int count = 0;
  ship.score += 10;
  for (int k = 0; k < size; k++) {
    if (!smallComets[k].isAlive) {
      count++;
      spawnSmallComet(parentComet, smallComets[k]);
      if (count == 2)
        break;
    }
  }
}
void onSmallCometCollide(Comet &parentComet, Comet tinyComets[], int size) {
  int count = 0;
  ship.score += 20;
  for (int i = 0; i < size; i++) {
    if (!tinyComets[i].isAlive) {
      count++;
      spawnSmallComet(parentComet, tinyComets[i]);
      if (count == 2)
        break;
    }
  }
}
void onTinyCometCollide(Comet &parentComet, Comet tinyComets[], int size) {
  ship.score += 30;
}

void checkForCollision(Comet cometArray[], Bullet bullets[],
                       int parentCometsSize,
                       void (*collisionHandler)(Comet &, Comet[], int),
                       Comet childComets[], int childCometsSize) {
  for (int i = 0; i < parentCometsSize; i++) {
    if (cometArray[i].isAlive)
      for (int j = 0; j < MAGAZINE_SIZE; j++) {
        if (bullets[j].isAlive) {
          if (pointToPolygonCollision(cometArray[i], bullets[j])) {
            cometArray[i].isAlive = false;
            bullets[j].isAlive = false;
            collisionHandler(cometArray[i], childComets, childCometsSize);

            continue;
          }
        }
      }
  }
}

void onShipCollide(Ship &ship) {
  ship.isAlive = false;
  ship.lives--;
}

void checkForShipCollision(Ship &ship, Comet cometArray[], int cometArraySize,
                           void (*ccollisionHandler)(Ship &ship)) {
  for (int i = 0; i < cometArraySize; i++) {
    bool check = shipToCometCollision(cometArray[i], ship);
    if (check) {
      onShipCollide(ship);
    }
  }
}
