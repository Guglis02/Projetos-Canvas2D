#ifndef __CILINDER_H__
#define __CILINDER_H__

#include <vector>
#include "../Utils/VectorHomo3d.h"
#include "../Utils/PointsUtils.h"
#include "./Model.h"

using namespace std;

class Cilinder : public Model
{
public:
    Cilinder(VectorHomo3d center, int steps, float height, float radius) : Model(center, steps)
    {
        this->radius = radius;
        this->height = height;
        this->div = div;
        this->Build();
    }
    ~Cilinder()
    {

    }

    void Build()
    {
        float theta = 0;

        for (int i = 0 ; i < steps; i++, theta += PI_2 / steps)
        {
            int h = 0;
            for (int j = 0; j < steps; j++, h += height / steps)
            {
                float x = radius * cos(theta);
                float y = radius * sin(theta);
                float z = h;
                points[i][j].x = x;
                points[i][j].y = y;
                points[i][j].z = z;
                points[i][j] += center;
            }
        }
    }
private:
    float verticeSize;
    float radius;
    float height;
    int div;
};

#endif
