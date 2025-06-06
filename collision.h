#ifndef _COLLISION_
#define _COLLISION_

#include "bullet.h"
#include "comet.h"

bool pointToPolygonCollision(Comet &comet, Bullet &bullet);
bool shipToCometCollision(Comet &comet, Ship &ship);

#endif
