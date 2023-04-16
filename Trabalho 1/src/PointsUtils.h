#ifndef POINTHANDLER_H_INCLUDED
#define POINTHANDLER_H_INCLUDED

#include <math.h>
#include <Vector2.h>

/** \brief
 * Arquivo utilitário contendo funções relacionadas a pontos.
 */

static int DistanceBetweenTwoPoints(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

static int DistanceBetweenTwoPoints(Vector2 p1, Vector2 p2)
{
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) * 1.0);
}

static float GetAngleBetweenPoints(Vector2 p1, Vector2 p2)
{
    float dotProduct = Vector2::DotProduct(p1, p2);
    float crossProduct = Vector2::CrossProduct(p1, p2);
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

// Adaptação usando Vector2
static int pnpoly(int nvert, Vector2* vert, Vector2 test)
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

// Calcula e retorna o ponto em segmento que é perpendicular ao segmento formado por vec1 e vec2, mais próximo de point
static Vector2 GetPerpendicularPoint(Vector2 vec1, Vector2 vec2, Vector2 point)
{
    Vector2 segment = vec2 - vec1;
    Vector2 pointOnSegment = vec1 + segment * (Vector2::DotProduct(point - vec1, segment)
                             / Vector2::DotProduct(segment, segment));
    return pointOnSegment;
}

static Vector2 RotatePoint(Vector2 point, float angle)
{
    return Vector2(point.x * cos(angle) - point.y * sin(angle),
                   point.x * sin(angle) + point.y * cos(angle));
}
#endif // POINTHANDLER_H_INCLUDED
