#ifndef __CILINDER_H__
#define __CILINDER_H__

#include <vector>
#include "../Utils/VectorHomo3d.h"
#include "../Utils/PointsUtils.h"
#include "./Model.h"

using namespace std;

// Classe responsável pelo modelo de um cilindro
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
                points[i][j].x = radius * cos(theta);
                points[i][j].y = radius * sin(theta);
                points[i][j].z = h;
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
