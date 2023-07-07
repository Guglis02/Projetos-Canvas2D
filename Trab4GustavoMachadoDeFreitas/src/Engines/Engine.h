#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <vector>
#include "../Utils/VectorHomo3d.h"
#include "../Utils/Matrix3d.h"
#include "../Utils/FpsController.h"
#include "../Utils/PointsUtils.h"
#include "../gl_canvas2d.h"

using namespace std;

class Engine
{
public:
    Engine(VectorHomo3d crankshaftAxis)
    {
        this->crankshaftAxis = crankshaftAxis;
        this->transformationMatrix = new Matrix3d();
    }
    ~Engine()
    {

    }
protected:
    Matrix3d* transformationMatrix;
    VectorHomo3d crankshaftAxis = VectorHomo3d(0, 0, 0);

    VectorHomo3d Rotate(VectorHomo3d v, float ang)
    {
        transformationMatrix->Reset();
        transformationMatrix->Translation(crankshaftAxis);
        transformationMatrix->RotationZ(ang);
        transformationMatrix->Translation(crankshaftAxis * -1);
        return *transformationMatrix * v;
    }
};

#endif
