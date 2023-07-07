#ifndef POINTUTILS_H_INCLUDED
#define POINTUTILS_H_INCLUDED

#include <math.h>
#include <vector>
#include "VectorHomo3d.h"
#include "GlobalConsts.h"

using namespace std;

static float GetAngleWithAxis(VectorHomo3d v)
{
    return atan2(v.y, v.x);
}

static float DegToRad(float deg)
{
    return deg * (PI / 180.0f);
}

static float RadToDeg(float rad)
{
    return rad * (180.0f / PI);
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
#endif // POINTUTILS_H_INCLUDED
