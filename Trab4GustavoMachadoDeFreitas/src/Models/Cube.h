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
    Cube(VectorHomo3d center, int steps, float verticeSize) : Model(center, steps)
    {
        this->verticeSize = verticeSize;
        this->Build();
    }
    ~Cube()
    {
        
    }

    void Build()
    {
        float halfVerticeSize = verticeSize * 0.5;
        float diagonal = sqrt(pow(halfVerticeSize, 2) + pow(halfVerticeSize, 2));

        float theta = DegToRad(45);
        float height = verticeSize;
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
    float verticeSize;
};

#endif
