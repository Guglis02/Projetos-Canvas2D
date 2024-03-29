#ifndef __COUNTERWEIGHT_H__
#define __COUNTERWEIGHT_H__

#include <vector>
#include "../Utils/VectorHomo3d.h"
#include "../Utils/PointsUtils.h"
#include "./Model.h"

using namespace std;

// Classe que representa o modelo trapezoide do contrapeso
class CounterWeight : public Model
{
public:
    CounterWeight(VectorHomo3d center, int steps, float height, float bigVerticeSize, float smallVerticeSize) : Model(center, steps)
    {
        this->bigVerticeSize = bigVerticeSize;
        this->smallVerticeSize = smallVerticeSize;
        this->height = height;
        this->Build();
    }
    ~CounterWeight()
    {

    }

    void Build()
    {
        float diagonal;
        float halfVerticeSize = bigVerticeSize * 0.5;
        float bigDiagonal = sqrt(pow(halfVerticeSize, 2) + pow(halfVerticeSize, 2));
        halfVerticeSize = smallVerticeSize * 0.5;
        float smallDiagonal = sqrt(pow(halfVerticeSize, 2) + pow(halfVerticeSize, 2));

        float theta = DegToRad(45);
        for (int i = 0 ; i < steps; i++, theta += (PI_2 / steps))
        {
            int h = 0;
            for (int j = 0; j < steps; j++, h += height / steps)
            {
                diagonal = (i != 1 && i != 2) ? smallDiagonal : bigDiagonal;               
                
                points[i][j].x = diagonal * cos(theta);
                points[i][j].y = diagonal * sin(theta);
                points[i][j].z = h;
                points[i][j] += center;
            }
        }
    }
private:
    float bigVerticeSize;
    float smallVerticeSize;
    float height;
};

#endif
