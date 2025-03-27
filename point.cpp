#include "point.h"
#include <cmath>

Point newPoint(float x, float y){
  Point p;
  p.x = x;
  p.y = y;
  return p;
}

Point rotateTransformPoint(Point p, float d){
  float radian = d *(M_PI /180.0);
  Point p2;
  p2.x = p.x * cos(p.x) - p.x * sin(p.x);
  p2.y = p.y * sin(p.x) + p.y * cos(p.x);
  return p2;
}
