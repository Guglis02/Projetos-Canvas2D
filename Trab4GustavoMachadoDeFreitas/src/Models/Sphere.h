#ifndef __SPHERE_H__
#define __SPHERE_H__

#include <vector>
#include "../Utils/VectorHomo3d.h"
#include "../Utils/PointsUtils.h"
#include "./Model.h"

using namespace std;

class Sphere : public Model
{
public:
    Sphere(VectorHomo3d center, int steps, float radius) : Model(center, steps)
    {
        this->center = center;
        this->radius = radius;
        this->Build();
    }
    ~Sphere()
    {
        
    }

    void Build()
    {
        int i = 0;
        int j = 0;
        float theta = 0;
        float phi = 0;
        
        for (i = 0 ; i < steps; i++, theta += PI_2 / steps)
        {
            phi = 0;
            for (j = 0; j < steps; j++, phi += PI / steps)
            {
                points[i][j].x = radius * cos(theta) * sin(phi);
                points[i][j].y = radius * sin(theta) * sin(phi);
                points[i][j].z = radius * cos(phi);
                points[i][j] += center;
            }
        }
    }
private:
    float radius;
};

#endif
