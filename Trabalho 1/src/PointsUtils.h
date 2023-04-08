#ifndef POINTHANDLER_H_INCLUDED
#define POINTHANDLER_H_INCLUDED

#include <math.h>

static int DistanceBetweenTwoPoints(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

// Retirado de: https://wrfranklin.org/Research/Short_Notes/pnpoly.html#The%20C%20Code
static int pnpoly(int nvert, float *vertx, float *verty, float testx, float testy)
{
  int i, j, c = 0;
  for (i = 0, j = nvert-1; i < nvert; j = i++) {
    if ( ((verty[i]>testy) != (verty[j]>testy)) &&
	 (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
       c = !c;
  }
  return c;
}

#endif // POINTHANDLER_H_INCLUDED
