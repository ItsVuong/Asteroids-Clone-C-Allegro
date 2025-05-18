#ifndef _MAIN_
#define _MAIN_

#include "comet.h"
#include <allegro5/allegro_font.h>
#include <allegro5/bitmap.h>
#include <allegro5/display.h>
#include <allegro5/events.h>
#include <allegro5/timer.h>

typedef void (*MenuHandler)(void); // Function pointer type

typedef struct {
  const char *text;
  MenuHandler handler;
} MENU_ITEM;

const int WIDTH = 1000;
const int HEIGHT = 700;
const int FPS = 60;

#endif
