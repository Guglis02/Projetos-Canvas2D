#ifndef POINTUTILS_H_INCLUDED
#define POINTUTILS_H_INCLUDED

#include <math.h>
#include "VectorHomo.h"

/** \brief
 * Arquivo utilit�rio contendo fun��es relacionadas a pontos.
 */

static int DistanceBetweenTwoPoints(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

static int DistanceBetweenTwoPoints(VectorHomo p1, VectorHomo p2)
{
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) * 1.0);
}

static float GetAngleBetweenPoints(VectorHomo p1, VectorHomo p2)
{
    float dotProduct = VectorHomo::DotProduct(p1, p2);
    float crossProduct = VectorHomo::CrossProduct(p1, p2);
    float angle = atan2(fabs(crossProduct), dotProduct);

    return crossProduct < 0 ? angle * -1.0 : angle;
}

// Retirado de: https://wrfranklin.org/Research/Short_Notes/pnpoly.html#The%20C%20Code
static int pnpoly(int nvert, float *vertx, float *verty, float testx, float testy)
{
    int i, j, c = 0;
    for (i = 0, j = nvert-1; i < nvert; j = i++)
    {
        if ( ((verty[i]>testy) != (verty[j]>testy)) &&
                (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
            c = !c;
    }
    return c;
}

// Adapta��o usando Vector2
static int pnpoly(int nvert, VectorHomo* vert, VectorHomo test)
{
    int i, j, c = 0;
    for (i = 0, j = nvert-1; i < nvert; j = i++)
    {
        if ( ((vert[i].y>test.y) != (vert[j].y>test.y)) &&
                (test.x < (vert[j].x-vert[i].x) * (test.y-vert[i].y) / (vert[j].y-vert[i].y) + vert[i].x) )
            c = !c;
    }
    return c;
}

// Calcula e retorna o ponto em segmento que � perpendicular ao segmento formado por vec1 e vec2, mais pr�ximo de point
static VectorHomo GetPerpendicularPoint(VectorHomo vec1, VectorHomo vec2, VectorHomo point)
{
    VectorHomo segment = vec2 - vec1;
    VectorHomo pointOnSegment = vec1 + segment * (VectorHomo::DotProduct(point - vec1, segment)
                             / VectorHomo::DotProduct(segment, segment));
    return pointOnSegment;
}

static VectorHomo RotatePoint(VectorHomo point, float angle)
{
    return VectorHomo(point.x * cos(angle) - point.y * sin(angle),
                   point.x * sin(angle) + point.y * cos(angle));
}
#endif // POINTUTILS_H_INCLUDED