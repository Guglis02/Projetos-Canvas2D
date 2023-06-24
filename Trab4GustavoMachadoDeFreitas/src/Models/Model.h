#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "../Utils/VectorHomo3d.h"
#include "../Utils/Matrix3d.h"

using namespace std;

class Model
{
public:
    Model()
    {
    }
    ~Model()
    {
        
    }

    vector<pair<int, int>> edges;
    vector<VectorHomo3d> vertices;

    virtual void Build(){};
    virtual void Draw(){};
protected:
    Matrix3d* transformationMatrix;
    vector<VectorHomo3d> transformedVertices;
};

#endif
