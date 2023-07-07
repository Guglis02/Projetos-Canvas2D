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

    void SetRpm(float value)
    {
        rpm = value;
    }

    float GetAngleBetweenPistons()
    {
        return fabs(leftChamberAng) + fabs(rightChamberAng);
    }

    float SetAngleBetweenPistons(float value)
    {
        leftChamberAng = value / 2;
        rightChamberAng = -value / 2;
    }

    void ToggleChamberView()
    {
        isShowingChamber = !isShowingChamber;
    }

    void TogglePistonView()
    {
        isShowingPiston = !isShowingPiston;
    }

    void ToggleCrankshaftView()
    {
        isShowingCrankshaft = !isShowingCrankshaft;
    }
protected:
    Matrix3d* transformationMatrix;
    VectorHomo3d crankshaftAxis = VectorHomo3d(0, 0, 0);

    bool isShowingChamber = true;
    bool isShowingPiston = true;
    bool isShowingCrankshaft = true;

    float crankshaftAng = 0;
    float leftChamberAng = 45;
    float rightChamberAng = -45;

    float rpm = 0;

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
