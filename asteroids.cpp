#include "asteroids.h"
#include "bullet.h"
#include "comet.h"
#include "point.h"
#include "ship.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
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

const int COMET_SIZE = 5;

int main() {
  bool running = true;
  bool redraw = true;
  bool keys[5] = {false, false, false, false, false};
  int angle = 0;

  // Objects
  Ship ship;
  Bullet bullets[MAGAZINE_SIZE];
  ALLEGRO_BITMAP *shipArrays[COMET_SIZE];
  Ship shipObjectArrays[COMET_SIZE];

  ALLEGRO_BITMAP *commetBitMaps[5];
  Comet cometArray[5];
  createShip(ship);
  createBullets(bullets);

  // Initialization functions
  al_init();
  al_init_primitives_addon();
  al_install_keyboard();

  // Allegro variables
  ALLEGRO_DISPLAY *display = al_create_display(WIDTH, HEIGHT);
  ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
  ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
  ALLEGRO_FONT *font = al_create_builtin_font();
  ALLEGRO_BITMAP *ship_image = al_create_bitmap(ship.width, ship.height);

  al_set_target_bitmap(ship_image);
  drawShip(ship);

  // for (int i = 0; i < 5; i++) {
  //   createShip(shipObjectArrays[i]);
  //   shipArrays[i] =
  //       al_create_bitmap(shipObjectArrays[i].width,
  //       shipObjectArrays[i].height);
  //   al_set_target_bitmap(shipArrays[i]);
  //   drawShip(shipObjectArrays[i]);
  // }

  for (int i = 0; i < 5; i++) {
    createComet(cometArray[i]);
    al_set_target_bitmap(cometArray[i].bitmap);
    drawComet(cometArray[i]);
  }

  for (int i = 0; i < 5; i++) {
    createShip(shipObjectArrays[i]);
    shipArrays[i] =
        al_create_bitmap(shipObjectArrays[i].width, shipObjectArrays[i].height);
    al_set_target_bitmap(shipArrays[i]);
    drawShip(shipObjectArrays[i]);
  }

  al_set_target_bitmap(al_get_backbuffer(display));

  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_timer_event_source(timer));
  al_register_event_source(event_queue, al_get_keyboard_event_source());
  al_start_timer(timer);

  Point randomPositions[5];
  for (int i = 0; i < 5; i++) {
    randomPositions[i].x = rand() % WIDTH;
    randomPositions[i].y = rand() % HEIGHT;
  }

  while (running) {
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
      running = false;
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
        fireBullet(bullets, ship);
        keys[SPACE] = false;
      }
      updateBullets(bullets);
      updateShip(ship);
      redraw = true;
    }

    // Only render every 1/60 second and when there are no event
    if (redraw && al_is_event_queue_empty(event_queue)) {
      al_clear_to_color(al_map_rgb(0, 0, 0));
      redraw = false;
      drawBullet(bullets, WIDTH, HEIGHT);

      for (int i = 0; i < COMET_SIZE; i++) {
        al_draw_rotated_bitmap(
            cometArray[i].bitmap, 100, 100, ship.coorX + randomPositions[i].x,
            ship.coorY + randomPositions[i].y, -ship.angle / 10, 0);
      }
      // Draw ship
      al_draw_rotated_bitmap(ship_image, ship.width / 2.0, ship.height / 2.0,
                             ship.coorX, ship.coorY,
                             -ship.angle * (M_PI / 180.0), 0);

      // Log to screen
      al_draw_textf(font, al_map_rgb(255, 255, 255), 15, 15, 0,
                    "Allegro is working! %f %f %f", ship.angle, sin(ship.angle),
                    cos(angle));
      al_draw_textf(font, al_map_rgb(255, 255, 255), 30, 30, 0,
                    "ship.x: %d, ship.y: %f", bullets[0].lifeTime,
                    ship.velocityY);
      al_flip_display();
    }
  }

  // Cleanup
  al_destroy_timer(timer);
  al_destroy_display(display);
  al_destroy_font(font);
  al_destroy_bitmap(ship_image);
  al_destroy_event_queue(event_queue); // Free event queue

  // Free dynamically created bitmaps
  for (int i = 0; i < COMET_SIZE; i++) {
    al_destroy_bitmap(shipArrays[i]);
    al_destroy_bitmap(cometArray[i].bitmap);
  }
}
