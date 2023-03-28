#ifndef POINTHANDLER_H_INCLUDED
#define POINTHANDLER_H_INCLUDED


static int DistanceBetweenTwoPoints(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

#endif // POINTHANDLER_H_INCLUDED
