#include "collision.h"
#include "comet.h"
#include "ship.h"
#include <cstddef>
#include <iostream>

bool pointToPolygonCollision(Comet &comet, Bullet &bullet) {
  int count = 0;

  if (bullet.isAlive && comet.isAlive) {
    for (size_t i = 0; i < comet.shape.size(); i++) {
      size_t next = (i + 1) % comet.shape.size();

      float x1 = getVertexLocation(comet, i).x;
      float y1 = getVertexLocation(comet, i).y;
      float x2 = getVertexLocation(comet, next).x;
      float y2 = getVertexLocation(comet, next).y;

      // Skip horizontal edges to avoid divide-by-zero
      if (y1 == y2)
        continue;

      bool intersect = (bullet.coorY < y1) != (bullet.coorY < y2);
      if (intersect) {
        float xIntersect = x1 + (bullet.coorY - y1) * (x2 - x1) / (y2 - y1);
        if (bullet.coorX < xIntersect) {
          count++;
        }
      }
    }
  }

  if (count % 2 == 0)
    return false;
  return true;
}

bool shipToCometCollision(Comet &comet, Ship &ship) {
  if (comet.isAlive)
    for (int i = 0; i < _SHIP_::SHIP_SHAPE.size(); i++) {
      int count = 0;
      for (size_t j = 0; j < comet.shape.size(); j++) {
        size_t next = (j + 1) % comet.shape.size();

        float x1 = getVertexLocation(comet, j).x;
        float y1 = getVertexLocation(comet, j).y;
        float x2 = getVertexLocation(comet, next).x;
        float y2 = getVertexLocation(comet, next).y;

        // Skip horizontal edges to avoid divide-by-zero
        if (y1 == y2)
          continue;

        Point shipCoor = _SHIP_::getVertexLocation(ship, i);
        bool intersect = (shipCoor.y < y1) != (shipCoor.y < y2);
        if (intersect) {
          float xIntersect =
              x1 + (shipCoor.y - y1) * (x2 - x1) / (y2 - y1);

          if (shipCoor.x < xIntersect) {
            count++;
          }
        }
      }
      if (count % 2 != 0)
        return true;
    }
  return false;
}
