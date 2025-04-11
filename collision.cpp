#include "collision.h"
#include <iostream>

bool pointToPolygonCollision(Comet &comet, Bullet &bullet) {
  int count = 0;

  if (bullet.isAlive && comet.isAlive) {
    for (size_t i = 0; i < comet.shape.size(); ++i) {
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
          std::cout << "Intersection count: " << count << std::endl;
        }
      }
    }
  }

  if(count % 2 == 0)
    return false;
  return true;
}
