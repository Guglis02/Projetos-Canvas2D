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
        vertices.push_back(VectorHomo3d(-50, -50, -50));
        vertices.push_back(VectorHomo3d(-50, -50, 50));
        vertices.push_back(VectorHomo3d(-50, 50, 50));
        vertices.push_back(VectorHomo3d(-50, 50, -50));
        vertices.push_back(VectorHomo3d(50, -50, -50));
        vertices.push_back(VectorHomo3d(50, -50, 50));
        vertices.push_back(VectorHomo3d(50, 50, 50));
        vertices.push_back(VectorHomo3d(50, 50, -50));

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
        transformationMatrix->Translation(GetCenter(vertices));
        transformationMatrix->RotationX(DegToRad(anglex));
        transformationMatrix->RotationY(DegToRad(angley));
        transformationMatrix->RotationZ(DegToRad(anglez));
        transformationMatrix->Translation(GetCenter(vertices) * -1);
        transformedVertices = transformationMatrix->ApplyToPoints(vertices);
    }

    void Draw()
    {
        CV::color(1, 0, 0);

        for (unsigned int i = 0; i < edges.size(); i++) {
            CV::line(transformedVertices[edges[i].first], transformedVertices[edges[i].second]);
        }
    }
};

#endif