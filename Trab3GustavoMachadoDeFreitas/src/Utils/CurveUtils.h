#ifndef CURVEUTILS_H_INCLUDED
#define CURVEUTILS_H_INCLUDED

#include "VectorHomo.h"

using namespace std;

// Arquivo com funções para cálculo de curvas

static VectorHomo Bezier3(VectorHomo p1, VectorHomo p2, VectorHomo p3, VectorHomo p4, float t)
{
    float tcube = t * t * t;
    float tsquare = t * t;

    return p1 * pow(1 - t, 3) 
        + p2 * 3 * t * pow(1 - t, 2) 
        + p3 * 3 * tsquare * (1 - t) 
        + p4 * tcube;
}

static VectorHomo BSpline3(VectorHomo p1, VectorHomo p2, VectorHomo p3, VectorHomo p4, float t)
{
    float asixth = 1.0f / 6.0f;
    float tcube = t * t * t;
    float tsquare = t * t;

    return p1 * asixth * pow((1 - t), 3) 
        + p2 * asixth * (3 * tcube - 6 * tsquare + 4) 
        + p3 * asixth * (-3 * tcube + 3 * tsquare + 3 * t + 1) 
        + p4 * asixth * tcube;
}

#endif // POINTUTILS_H_INCLUDED