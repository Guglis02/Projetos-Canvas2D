#ifndef POINTUTILS_H_INCLUDED
#define POINTUTILS_H_INCLUDED

#include <math.h>
#include <vector>
#include "VectorHomo.h"
#include "VectorHomo3d.h"
#include "GlobalConsts.h"

using namespace std;

// Retorna o angulo em radianos entre o vetor e o eixo x
static float GetAngleWithXAxis(VectorHomo3d v)
{
    return atan2(v.y, v.x);
}

static float DegToRad(float deg)
{
    return deg * PI / 180.0f;
}

static VectorHomo3d GetCenter(vector<VectorHomo3d> points)
{
    VectorHomo3d center = VectorHomo3d(0, 0, 0);
    for (int i = 0; i < points.size(); i++)
    {
        center = center + points[i];
    }
    center = center / points.size();
    return center;
}

static VectorHomo3d GetCenter(vector<vector<VectorHomo3d>> points)
{
    VectorHomo3d center = VectorHomo3d(0, 0, 0);
    for (int i = 0; i < points.size(); i++)
    {
        for (int j = 0; j < points[i].size(); j++)
        {
            center = center + points[i][j];
        }
    }
    center = center / (points.size() * points[0].size());
    return center;
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
