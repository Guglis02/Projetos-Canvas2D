#ifndef __CUBE_H__
#define __CUBE_H__

#include <vector>
#include "../Utils/VectorHomo3d.h"
#include "./Model.h"

using namespace std;

class Cube : public Model
{
public:
    Cube() : Model()
    {
        this->Build();
        transformationMatrix = new Matrix3d();
    }
    ~Cube()
    {
        
    }

    void Build()
    {
        vertices.push_back(new VectorHomo3d(-50, -50, -50));
        vertices.push_back(new VectorHomo3d(-50, -50, 50));
        vertices.push_back(new VectorHomo3d(-50, 50, 50));
        vertices.push_back(new VectorHomo3d(-50, 50, -50));
        vertices.push_back(new VectorHomo3d(50, -50, -50));
        vertices.push_back(new VectorHomo3d(50, -50, 50));
        vertices.push_back(new VectorHomo3d(50, 50, 50));
        vertices.push_back(new VectorHomo3d(50, 50, -50));

        std::vector<std::pair<int, int>> edges = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0},
            {4, 5}, {5, 6}, {6, 7}, {7, 4},
            {0, 4}, {1, 5}, {2, 6}, {3, 7}
        };

        this->edges = edges;
    }

    void Transform(int anglex, int angley, int anglez)
    {
        transformedVertices.clear();
        transformationMatrix->Reset();
        transformationMatrix->RotationX(anglex * PI / 180.0f);
        transformationMatrix->RotationY(angley * PI / 180.0f);
        transformationMatrix->RotationZ(anglez * PI / 180.0f);
        transformedVertices = transformationMatrix->ApplyToPoints(vertices);
    }

    void Draw()
    {
        CV::color(1, 0, 0);

        for (unsigned int i = 0; i < edges.size(); i++) {
            CV::line(*transformedVertices[edges[i].first], *transformedVertices[edges[i].second]);
        }
    }
};

#endif
