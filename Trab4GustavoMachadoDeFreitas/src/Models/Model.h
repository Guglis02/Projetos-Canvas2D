#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "../Utils/VectorHomo3d.h"
#include "../Utils/Matrix3d.h"

using namespace std;

class Model
{
public:
    Model(VectorHomo3d center)
    {
        this->center = center;
        transformationMatrix = new Matrix3d();
    }
    ~Model()
    {
        delete transformationMatrix;
    }

    VectorHomo3d GetCenter()
    {
        return center;
    }

    void LocalRotate(int anglex, int angley, int anglez, bool isPerm)
    {
        transformationMatrix->Reset();
        transformationMatrix->Translation(center);
        transformationMatrix->RotationX(DegToRad(anglex));
        transformationMatrix->RotationY(DegToRad(angley));
        transformationMatrix->RotationZ(DegToRad(anglez));
        transformationMatrix->Translation(center * -1);
            
        transformedVertices = transformationMatrix->ApplyToPoints(vertices);

        if (isPerm)
        {
            vertices = transformationMatrix->ApplyToPoints(vertices);
        }
    }

    void Reposition(VectorHomo3d newCenter, bool isPerm)
    {
        transformationMatrix->Reset();
        transformationMatrix->Translation(newCenter);

        transformedVertices = transformationMatrix->ApplyToPoints(vertices);

        if (isPerm)
        {
            vertices = transformationMatrix->ApplyToPoints(vertices);
        }
    }

    void GlobalRotate(int anglex, int angley, int anglez)
    {
        transformationMatrix->Reset();
        transformationMatrix->RotationX(DegToRad(anglex));
        transformationMatrix->RotationY(DegToRad(angley));
        transformationMatrix->RotationZ(DegToRad(anglez));
        
        transformedVertices = transformationMatrix->ApplyToPoints(transformedVertices);
    }

    vector<pair<int, int>> edges;
    vector<VectorHomo3d> vertices;

    virtual void Build(){};
    virtual void Draw(){};
protected:
    Matrix3d* transformationMatrix;
    vector<VectorHomo3d> transformedVertices;
    VectorHomo3d center;
};

#endif
