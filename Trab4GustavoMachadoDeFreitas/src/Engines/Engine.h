#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <vector>
#include "../Utils/VectorHomo3d.h"
#include "../Utils/Matrix3d.h"
#include "../Utils/FpsController.h"
#include "../Utils/PointsUtils.h"
#include "../gl_canvas2d.h"

using namespace std;

// Classe base de um motor, guarda informações em comum dos motores 2D e 3D
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

    virtual void Render(){};

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

    // Eixo base do virabrequim, setado no construtor dos motores
    VectorHomo3d crankshaftAxis = VectorHomo3d(0, 0, 0);

    // Controladores de qual parte do motor deve ser mostrada
    bool isShowingChamber = true;
    bool isShowingPiston = true;
    bool isShowingCrankshaft = true;

    float crankshaftAng = 0;

    // Angulos dos pistões
    float leftChamberAng = 45;
    float rightChamberAng = -45;

    float rpm = 0;

    // Roda o virabrequim
    virtual void SpinCrankshaft(){};

    // Lida com cada lado do motor individualmente
    virtual void LeftPart(){};
    virtual void RightPart(){};

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
