#include "main.h"
#include "game.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/bitmap.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/color.h>
#include <allegro5/display.h>
#include <allegro5/drawing.h>
#include <allegro5/keyboard.h>
#include <allegro5/keycodes.h>
#include <allegro5/system.h>
#include <csignal>

bool running = true;
bool runGame = false;
int currentOption = 0;
const int MENU_ITEM_SIZE = 2;

void onPlayButtonSelect() { runGame = true; }
void onCloseButtonSelect() { running = false; }

void menuItemIncrement() {
  if (currentOption < MENU_ITEM_SIZE - 1) {
    currentOption++;
  }
}

void menuItemDecrement() {
  if (currentOption > 0) {
    currentOption--;
  }
}

int main() {
  // Initialize Allegro
  if (!al_init() || !al_install_keyboard() || !al_init_primitives_addon() ||
      !al_init_ttf_addon()) {
    return -1;
  }

  // Create resources
  ALLEGRO_DISPLAY *display = al_create_display(WIDTH, HEIGHT);
  ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
  ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
  ALLEGRO_FONT *font = al_create_builtin_font();
  ALLEGRO_FONT *titleFont =
      al_load_ttf_font("assets/ASTEROID TYPE demo.ttf", 220, 0);
  ALLEGRO_FONT *menuFont = al_load_ttf_font("assets/DePixelKlein.ttf", 20, 0);

  if (!display || !timer || !event_queue || !font || !titleFont || !menuFont) {
    return -1;
  }

  // Event sources
  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_keyboard_event_source());
  al_register_event_source(event_queue, al_get_timer_event_source(timer));
  al_start_timer(timer);

  // Create arrow bitmap
  ALLEGRO_BITMAP *arrow = al_create_bitmap(10, 10);
  if (!arrow) {
    return -1;
  }
  al_set_target_bitmap(arrow);
  al_clear_to_color(al_map_rgba(0, 0, 0, 0));
  al_draw_line(0, 0, 10, 5, al_map_rgb(255, 255, 255), 2);
  al_draw_line(0, 10, 10, 5, al_map_rgb(255, 255, 255), 2);
  al_set_target_bitmap(al_get_backbuffer(display));

  MENU_ITEM menuItem[] = {{.text = "PLAY", .handler = onPlayButtonSelect},
                          {.text = "CLOSE", .handler = onCloseButtonSelect}};

  // Main loop
  while (running) {
    // Menu loop
    while (!runGame && running) {
      ALLEGRO_EVENT event;
      al_wait_for_event(event_queue, &event);

      if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        running = false;
        break;
      }

      if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (event.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
          menuItemDecrement();
          break;
        case ALLEGRO_KEY_DOWN:
          menuItemIncrement();
          break;
        case ALLEGRO_KEY_ENTER:
          menuItem[currentOption].handler();
          break;
        }
      }

      if (al_is_event_queue_empty(event_queue)) {
        al_clear_to_color(al_map_rgb(0, 0, 0));

        al_draw_text(titleFont, al_map_rgb(255, 255, 255), WIDTH / 2.0, 100,
                     ALLEGRO_ALIGN_CENTER, "ASTEROID");

        for (int i = 0; i < MENU_ITEM_SIZE; i++) {
          al_draw_text(menuFont, al_map_rgb(255, 255, 255), WIDTH / 2.0,
                       400 + 25 * i, ALLEGRO_ALIGN_CENTER, menuItem[i].text);
        }

        al_draw_bitmap(
            arrow,
            WIDTH / 2.0 -
                al_get_text_width(menuFont, menuItem[currentOption].text) /
                    2.0 -
                30,
            403 + 25 * currentOption, 0);

        al_flip_display();
      }
    }

    // If running is still true, enter the game
    if (running) {
      game(display, timer, event_queue, font);
      runGame = false; // after the game ends, return to menu
    }
  }

  // Clean up
  al_destroy_bitmap(arrow);
  al_destroy_font(font);
  al_destroy_font(titleFont);
  al_destroy_font(menuFont);
  al_destroy_timer(timer);
  al_destroy_event_queue(event_queue);
  al_destroy_display(display);

  return 0;
}
