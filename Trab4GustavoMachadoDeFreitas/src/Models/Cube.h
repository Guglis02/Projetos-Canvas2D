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
    Cube(VectorHomo3d center, float verticeSize) : Model(center)
    {
        this->verticeSize = verticeSize;
        this->Build();
    }
    ~Cube()
    {
        
    }

    void Build()
    {
        float halfSize = verticeSize * 0.5f;

        vertices.push_back(VectorHomo3d(center.x - halfSize, center.y - halfSize, center.z - halfSize));
        vertices.push_back(VectorHomo3d(center.x - halfSize, center.y - halfSize, center.z + halfSize));
        vertices.push_back(VectorHomo3d(center.x - halfSize, center.y + halfSize, center.z + halfSize));
        vertices.push_back(VectorHomo3d(center.x - halfSize, center.y + halfSize, center.z - halfSize));
        vertices.push_back(VectorHomo3d(center.x + halfSize, center.y - halfSize, center.z - halfSize));
        vertices.push_back(VectorHomo3d(center.x + halfSize, center.y - halfSize, center.z + halfSize));
        vertices.push_back(VectorHomo3d(center.x + halfSize, center.y + halfSize, center.z + halfSize));
        vertices.push_back(VectorHomo3d(center.x + halfSize, center.y + halfSize, center.z - halfSize));

        std::vector<std::pair<int, int>> edges = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0},
            {4, 5}, {5, 6}, {6, 7}, {7, 4},
            {0, 4}, {1, 5}, {2, 6}, {3, 7}
        };

        this->edges = edges;

        transformedVertices = vertices;
    }

    void Draw()
    {
        CV::color(1, 0, 0);

        for (unsigned int i = 0; i < edges.size(); i++) {
            CV::line(transformedVertices[edges[i].first], transformedVertices[edges[i].second]);
        }
    }
private:
    float verticeSize;
};

#endif
