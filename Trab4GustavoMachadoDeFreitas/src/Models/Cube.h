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
      
        vector<VectorHomo3d> edges; 
        // Generate the bottom square, and then uses for loops to iterate over the square while incrementing z, to generate the walls of the cube. Then stores the points in the points array.
        edges.push_back(VectorHomo3d(-halfVerticeSize, -halfVerticeSize, -halfVerticeSize) + center);
        edges.push_back(VectorHomo3d(halfVerticeSize, -halfVerticeSize, -halfVerticeSize) + center);
        edges.push_back(VectorHomo3d(halfVerticeSize, halfVerticeSize, -halfVerticeSize) + center);
        edges.push_back(VectorHomo3d(-halfVerticeSize, halfVerticeSize, -halfVerticeSize) + center);
        
        float t = 0;
        for (int i = 0; i < steps; i++, t += (1 / steps))
        {
            int h = 0;
            for (int j = 0; j < steps; j++, h += (verticeSize / steps))
            {
                float x = (edges[0] * t + edges[1] * (1 - t)).x;
                float y = (edges[1] * t + edges[2] * (1 - t)).y;
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
