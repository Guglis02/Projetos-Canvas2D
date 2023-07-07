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

#endif // POINTUTILS_H_INCLUDED
