#ifndef __CUBE_H__
#define __CUBE_H__

#include <vector>
#include "../Utils/VectorHomo3d.h"
#include "../Utils/PointsUtils.h"
#include "./Model.h"

using namespace std;

class Cube : public Model
{
public:
    Cube(VectorHomo3d center, int steps, float verticeSize, float height) : Model(center, steps)
    {
        this->verticeSize = verticeSize;
        this->height = height;
        this->Build();
    }
    ~Cube()
    {
        
    }

    VectorHomo3d GetDirection()
    {

    }

    void Build()
    {
        float halfVerticeSize = verticeSize * 0.5;
        float diagonal = sqrt(pow(halfVerticeSize, 2) + pow(halfVerticeSize, 2));

        float theta = DegToRad(45);
        float height = this->height;
        for (int i = 0 ; i < steps; i++, theta += (PI_2 / steps))
        {
            int h = 0;
            for (int j = 0; j < steps; j++, h += height / steps)
            {
                points[i][j].x = diagonal * cos(theta);
                points[i][j].y = h;
                points[i][j].z = diagonal * sin(theta);
                points[i][j] += center;
            }
        }
    }
private:
    float verticeSize;
    float height;
};

#endif
