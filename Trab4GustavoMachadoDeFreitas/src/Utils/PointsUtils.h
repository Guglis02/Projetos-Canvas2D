#ifndef POINTUTILS_H_INCLUDED
#define POINTUTILS_H_INCLUDED

#include <math.h>
#include "VectorHomo.h"

// Retorna o angulo em radianos entre o vetor e o eixo x
static float GetAngleWithXAxis(VectorHomo v)
{
    return atan2(v.y, v.x);
}

// Retirado de: https://wrfranklin.org/Research/Short_Notes/pnpoly.html#The%20C%20Code
// Adapta��o usando Vector2
static int pnpoly(int nvert, VectorHomo *vert, VectorHomo test)
{
    int i, j, c = 0;
    for (i = 0, j = nvert - 1; i < nvert; j = i++)
    {
        if (((vert[i].y > test.y) != (vert[j].y > test.y)) &&
            (test.x < (vert[j].x - vert[i].x) * (test.y - vert[i].y) / (vert[j].y - vert[i].y) + vert[i].x))
            c = !c;
    }
    return c;
}
#endif // POINTUTILS_H_INCLUDED
