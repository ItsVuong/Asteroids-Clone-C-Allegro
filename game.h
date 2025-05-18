#ifndef _ASTEROIDS_
#define _ASTEROIDS_

#include <allegro5/allegro_font.h>
#include <allegro5/events.h>
enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };
int game(ALLEGRO_DISPLAY *display, ALLEGRO_TIMER *timer,
         ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_FONT *font);

#endif // !_ASTEROIDS_
