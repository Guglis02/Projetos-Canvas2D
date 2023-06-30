#ifndef __COUNTERWEIGHT_H__
#define __COUNTERWEIGHT_H__

#include <vector>
#include "../Utils/VectorHomo3d.h"
#include "../Utils/PointsUtils.h"
#include "./Model.h"

using namespace std;

class CounterWeight : public Model
{
public:
    CounterWeight(VectorHomo3d center, int steps, float height, float bigVerticeSize, float smallVerticeSizee) : Model(center, steps)
    {
        this->bigVerticeSize = bigVerticeSize;
        this->smallVerticeSize = smallVerticeSize;
        this->radius = radius;
        this->height = height;
        this->div = div;
        this->Build();
    }
    ~CounterWeight()
    {

    }

    void Build()
    {
        float halfVerticeSize = bigVerticeSize * 0.5;
        float smallDiagonal = sqrt(pow(halfVerticeSize, 2) + pow(halfVerticeSize, 2));

        float theta = DegToRad(45);
        for (int i = 0 ; i < steps; i++, theta += (PI_2 / steps))
        {
            int h = 0;
            for (int j = 0; j < steps; j++, h += height / steps)
            {
                float x = diagonal * cos(theta);
                float y = diagonal * sin(theta);
                float z = h;
                points[i][j].x = x;
                points[i][j].y = y;
                points[i][j].z = z;
                points[i][j] += center;
            }
        }
    }
private:
    float bigVerticeSize;
    float smallVerticeSize;
    float height;
    int div;
};

#endif
