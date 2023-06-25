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
    Cilinder(VectorHomo3d center, float height, float radius, int div) : Model(center)
    {
        this->verticeSize = verticeSize;
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
        float theta = PI_2 / div;

        vector<VectorHomo3d> topVertices;
        vector<VectorHomo3d> bottomVertices;
        for (int i = 0; i < div; i++) {
            float x = radius * cos(theta * i);
            float z = radius * sin(theta * i);
            topVertices.push_back(VectorHomo3d(x, height/2, z));
            bottomVertices.push_back(VectorHomo3d(x, -height/2, z));
        }

        for (int i = 0; i < div; i++) {
            int j = (i + 1) % div;
            edges.push_back(make_pair(i, j));
            edges.push_back(make_pair(i + div, j + div));
            edges.push_back(make_pair(i, i + div));
        }
        vertices = topVertices;
        vertices.insert(vertices.end(), bottomVertices.begin(), bottomVertices.end());
    }

    void Transform(int anglex, int angley, int anglez)
    {
        transformedVertices.clear();
        transformationMatrix->Reset();
        transformationMatrix->Translation(center);
        transformationMatrix->RotationX(DegToRad(anglex));
        transformationMatrix->RotationY(DegToRad(angley));
        transformationMatrix->RotationZ(DegToRad(anglez));
        transformationMatrix->Translation(center * -1);
        transformedVertices = transformationMatrix->ApplyToPoints(vertices);
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
    float radius;
    float height;
    int div;
};

#endif
