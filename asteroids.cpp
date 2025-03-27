#include "bullet.h"
#include "ship.h"
#include "comet.h"
#include "point.h"
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

const int WIDTH = 1000;
const int HEIGHT = 700;
const float FPS = 60.0;

enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };

int main() {
  bool running = true;
  bool redraw = true;
  bool keys[5] = {false, false, false, false, false};
  int angle = 0;

  // Objects
  Ship ship;
  Bullet bullets[MAGAZINE_SIZE];
  Point p1 = {500, 500};

  // Allegro variables
  ALLEGRO_DISPLAY *display = NULL;
  ALLEGRO_TIMER *timer = NULL;
  ALLEGRO_EVENT_QUEUE *event_queue = NULL;
  ALLEGRO_FONT *font = NULL;
  ALLEGRO_BITMAP *image = NULL;

  // Initialization functions
  al_init();
  al_init_primitives_addon();
  al_install_keyboard();

  createShip(ship);
  createBullets(bullets);

  display = al_create_display(WIDTH, HEIGHT);
  event_queue = al_create_event_queue();
  timer = al_create_timer(1.0 / FPS);
  font = al_create_builtin_font();
  image = al_create_bitmap(ship.width, ship.height);
  al_set_target_bitmap(image);
  drawShip(ship);
  // al_clear_to_color(al_map_rgb(255, 255, 255));
  al_set_target_bitmap(al_get_backbuffer(display));

  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_timer_event_source(timer));
  al_register_event_source(event_queue, al_get_keyboard_event_source());
  al_start_timer(timer);
  while (running) {
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);
    // Read keyboard events
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
      switch (event.keyboard.keycode) {
      case ALLEGRO_KEY_UP:
        keys[UP] = true;
        break;
      case ALLEGRO_KEY_LEFT:
        keys[LEFT] = true;
        break;
      case ALLEGRO_KEY_RIGHT:
        keys[RIGHT] = true;
        break;
      case ALLEGRO_KEY_SPACE:
        keys[SPACE] = true;
        break;
      }
    }
    if (event.type == ALLEGRO_EVENT_KEY_UP) {
      switch (event.keyboard.keycode) {
      case ALLEGRO_KEY_UP:
        keys[UP] = false;
        break;
      case ALLEGRO_KEY_LEFT:
        keys[LEFT] = false;
        break;
      case ALLEGRO_KEY_RIGHT:
        keys[RIGHT] = false;
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
      friction(ship);
      //  Set ship's position
      ship.coorY += -ship.velocityY;
      ship.coorX += ship.velocityX;

      // Re-position spaceship if it goes out of screen
      if (ship.coorY < (-HEIGHT - ship.width) / 2.0)
        ship.coorY = (HEIGHT + ship.width) / 2.0;
      if (ship.coorY > (HEIGHT + ship.width) / 2.0)
        ship.coorY = (-HEIGHT - ship.width) / 2.0;
      if (ship.coorX < (-WIDTH - ship.width) / 2.0)
        ship.coorX = (WIDTH + ship.width) / 2.0;
      if (ship.coorX > (WIDTH + ship.width) / 2.0)
        ship.coorX = (-WIDTH - ship.width) / 2.0;
      redraw = true;
    }

    // Only render every 1/60 second and when there are no event
    if (redraw && al_is_event_queue_empty(event_queue)) {
      redraw = false;
      al_clear_to_color(al_map_rgb(0, 0, 0));
      al_draw_line(400, 400, p1.x, p1.y, al_map_rgb(255, 255, 255), 3);
      p1=rotateTransformPoint(p1, 0.000001);
      drawBullet(bullets, WIDTH, HEIGHT);
      drawComet();
      al_draw_rotated_bitmap(
          image, ship.width / 2.0, ship.height / 2.0, ship.coorX + WIDTH / 2.0,
          ship.coorY + HEIGHT / 2.0, -ship.angle * (M_PI / 180.0), 0);
      // Log to screen
      al_draw_textf(font, al_map_rgb(255, 255, 255), 15, 15, 0,
                    "Allegro is working! %f %f %f", ship.angle, sin(ship.angle),
                    cos(angle));
      al_draw_textf(font, al_map_rgb(255, 255, 255), 30, 30, 0,
                    "ship.x: %f, ship.y: %f", ship.velocityX, ship.velocityY);
      al_flip_display();
    }
  }

  al_destroy_timer(timer);
  al_destroy_display(display);
  al_destroy_font(font);
  al_destroy_bitmap(image);
  al_destroy_event_queue(event_queue);
}
