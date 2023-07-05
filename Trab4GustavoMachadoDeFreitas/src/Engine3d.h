#ifndef __ENGINE3D_H__
#define __ENGINE3D_H__

#include <vector>
#include "./Utils/VectorHomo3d.h"
#include "./Utils/Matrix3d.h"
#include "./Utils/FpsController.h"
#include "./Utils/PointsUtils.h"
#include "./Models/Cube.h"
#include "./Models/Cilinder.h"
#include "./Models/CounterWeight.h"
#include "./Models/Model.h"
#include "gl_canvas2d.h"

using namespace std;

class Engine3d
{
public:
    Engine3d(VectorHomo3d crankshaftAxis, bool isFlipped, float startAngle = 0)
    {
        this->crankshaftAxis = crankshaftAxis;
        this->invertionCoef = isFlipped ? -1 : 1;
        this->ang = startAngle;
        transformationMatrix = new Matrix3d();

        int chamberX = 200 * invertionCoef;
        int chamberAng = invertionCoef == 1 ? DegToRad(240) : DegToRad(60) * -1;
        chamber = new Cube(crankshaftAxis + VectorHomo3d(chamberX, 250, 0), 4, 150, 150);
        chamber->LocalRotate(0, 0, chamberAng, true);
        parts.push_back(chamber);

        rotatingPoint = crankshaftAxis + VectorHomo3d(crankShaftAxisRadius * 2 * cos(ang), crankShaftAxisRadius * 2 * sin(ang), 0);
        crankPin = new Cilinder(rotatingPoint, 20, crankShaftAxisRadius, crankShaftAxisRadius);
        parts.push_back(crankPin);

        mainJournal = new Cilinder(crankshaftAxis, 20, crankShaftAxisRadius, crankShaftAxisRadius);
        parts.push_back(mainJournal);

        counterWeight = new CounterWeight(crankshaftAxis, 4, 20, 8 * crankShaftAxisRadius, 3 * crankShaftAxisRadius);
        parts.push_back(counterWeight);

        connectingRod = new Cilinder(rotatingPoint, 20, connectingRodLength, 5);
        connectingRod->LocalRotate(0, DegToRad(90), 0, true);
        parts.push_back(connectingRod);

        piston = new Cube(crankshaftAxis + VectorHomo3d(chamberX, 250, 0), 4, 150, 40);        
        piston->LocalRotate(0, 0, chamberAng, true);
        parts.push_back(piston);
    }
    ~Engine3d()
    {

    }

    void Render(float anglex, float angley, float anglez, int d)
    {
        for (auto part : parts)
        {
            part->ResetTransformedPoints();
        }

        SpinCrankshaft();

        // Calculates the piston joint
        chamberBase = chamber->GetCenter();
        VectorHomo3d dir = chamberBase - rotatingPoint;
        dir.normalize();

        connectingRod->Reposition(rotatingPoint, true);
        connectingRod->LocalRotate(0, 0, GetAngleWithAxis(dir), false);

        pistonJoint = rotatingPoint + (dir * connectingRodLength);

        CV::color(1, 0, 0);
        for (auto part : parts)
        {
            part->GlobalRotate(anglex, angley, anglez, crankshaftAxis);
            part->DrawOrthogonal();
        }

        // DrawPiston();
    }
private:
    Matrix3d* transformationMatrix;

    vector<Model*> parts;
    Cube* chamber;
    Cube* piston;
    Cilinder* mainJournal;
    Cilinder* crankPin;
    Cilinder* connectingRod;
    CounterWeight* counterWeight;

    VectorHomo3d rotatingPoint;
    VectorHomo3d chamberBase;
    VectorHomo3d pistonJoint = VectorHomo3d(0, 200, 0);

    float ang = 0;
    int crankShaftAxisRadius = 30;
    int connectingRodLength = 240;

    VectorHomo3d crankshaftAxis = VectorHomo3d(0, 0, 0);

    int invertionCoef = 1;

    // Gira a manivela
    void SpinCrankshaft()
    {
        ang += (1.5 * FpsController::getInstance().GetDeltaTime());
        ang = ang > PI_2 ? 0 : ang;
        rotatingPoint = crankshaftAxis + VectorHomo3d(crankShaftAxisRadius * 2 * cos(ang), crankShaftAxisRadius * 2 * sin(ang), 0);
        crankPin->Reposition(rotatingPoint, true);

        counterWeight->LocalRotate(0, 0, ang, false, crankshaftAxis);
        
        mainJournal->LocalRotate(0, 0, ang, false);
    }

    // void DrawPiston()
    // {
    //     // Calcula linha oposta ao pistão
    //     VectorHomo3d chamberDir = chamber[1] - chamber[0];
    //     chamberDir.normalize();

    //     VectorHomo3d chamberMiddle = (chamber[0] + chamber[1]) * 0.5f;

    //     // Linha perpendicular ao fundo da câmara
    //     VectorHomo3d perpDir = VectorHomo3d(-chamberDir.y, chamberDir.x, 0);

    //     // Calcula a projeção do pistão na câmara
    //     VectorHomo3d pistonProj = pistonJoint - chamberMiddle;
    //     float projLength = pistonProj * (perpDir);
    //     VectorHomo3d pistonProjDir = perpDir * projLength;
    //     VectorHomo3d pistonProjPoint = chamberMiddle + pistonProjDir;

    //     // Calcula as pontas do pistão
    //     VectorHomo3d pistonStart = pistonProjPoint - (chamberDir * 50);
    //     VectorHomo3d pistonEnd = pistonProjPoint + (chamberDir * 50);

    //     // Desenha o pistão
    //     CV::line(pistonStart, pistonEnd);
    // }
};

#endif
