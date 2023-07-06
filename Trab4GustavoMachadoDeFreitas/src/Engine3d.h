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
    Engine3d(VectorHomo3d crankshaftAxis)
    {
        this->crankshaftAxis = crankshaftAxis;
        transformationMatrix = new Matrix3d();

        chamberBase = crankshaftAxis + VectorHomo3d(0, 250, 0);
        leftChamber = new Cube(chamberBase, 4, 150, 150);
        parts.push_back(leftChamber);

        rightChamber = new Cube(chamberBase, 4, 150, 150);
        parts.push_back(rightChamber);

        rotatingPoint = crankshaftAxis + VectorHomo3d(crankShaftAxisRadius * 2 * cos(crankshaftAng), crankShaftAxisRadius * 2 * sin(crankshaftAng), 0);
        crankPin = new Cilinder(rotatingPoint + crankPinOffset, 30, 55, crankShaftAxisRadius);
        parts.push_back(crankPin);

        backMainJournal = new Cilinder(crankshaftAxis + VectorHomo3d(0, 0, 15), 20, 150, crankShaftAxisRadius);
        parts.push_back(backMainJournal);

        frontMainJournal = new Cilinder(crankshaftAxis + VectorHomo3d(0, 0, -15), 20, 150, crankShaftAxisRadius);
        frontMainJournal->LocalRotate(0, DegToRad(180), 0, true);
        parts.push_back(frontMainJournal);

        backCounterWeight = new CounterWeight(crankshaftAxis + VectorHomo3d(0, 0, 15), 4, 20, 8 * crankShaftAxisRadius, 3 * crankShaftAxisRadius);
        parts.push_back(backCounterWeight);
        
        frontCounterWeight = new CounterWeight(crankshaftAxis + VectorHomo3d(0, 0, -25), 4, 20, 8 * crankShaftAxisRadius, 3 * crankShaftAxisRadius);
        parts.push_back(frontCounterWeight);

        leftConnectingRod = new Cilinder(rotatingPoint, 20, connectingRodLength, 5);
        leftConnectingRod->LocalRotate(0, DegToRad(90), 0, true);
        parts.push_back(leftConnectingRod);

        rightConnectingRod = new Cilinder(rotatingPoint, 20, connectingRodLength, 5);
        rightConnectingRod->LocalRotate(0, DegToRad(90), 0, true);
        parts.push_back(rightConnectingRod);

        leftPiston = new Cube(chamberBase, 4, 150, pistonHeight);
        leftPiston->LocalRotate(0, 0, DegToRad(leftChamberAng), true);
        parts.push_back(leftPiston);
        
        rightPiston = new Cube(chamberBase, 4, 150, pistonHeight);
        rightPiston->LocalRotate(0, 0, DegToRad(rightChamberAng), true);
        parts.push_back(rightPiston);
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
        LeftPart();
        RightPart();

        CV::color(1, 0, 0);
        for (auto part : parts)
        {
            part->GlobalRotate(anglex, angley, anglez, crankshaftAxis);
            part->DrawOrthogonal();
        }
    }
private:
    Matrix3d* transformationMatrix;

    vector<Model*> parts;
    Cube* leftChamber;
    Cube* rightChamber;
    Cube* leftPiston;
    Cube* rightPiston;
    Cilinder* backMainJournal;
    Cilinder* frontMainJournal;
    Cilinder* crankPin;
    Cilinder* leftConnectingRod;
    Cilinder* rightConnectingRod;
    CounterWeight* frontCounterWeight;
    CounterWeight* backCounterWeight;

    VectorHomo3d rotatingPoint;
    VectorHomo3d chamberBase;
    VectorHomo3d pistonJoint = VectorHomo3d(0, 200, 0);
    VectorHomo3d crankPinOffset = VectorHomo3d(0, 0, -15);

    float crankshaftAng = 0;
    float leftChamberAng = 45;
    float rightChamberAng = -45;

    int crankShaftAxisRadius = 30;
    int connectingRodLength = 290;
    int pistonHeight = 40;

    VectorHomo3d crankshaftAxis = VectorHomo3d(0, 0, 0);

    VectorHomo3d Rotate(VectorHomo3d v, float ang)
    {
        transformationMatrix->Reset();
        transformationMatrix->Translation(crankshaftAxis);
        transformationMatrix->RotationZ(ang);
        transformationMatrix->Translation(crankshaftAxis * -1);
        return *transformationMatrix * v;
    }

    void LeftPart()
    {
        VectorHomo3d correctedRotationPoint = Rotate(rotatingPoint, DegToRad(-1 * leftChamberAng));        
        float jointY = correctedRotationPoint.y + sqrt(pow(connectingRodLength - pistonHeight * 0.5, 2) - pow(correctedRotationPoint.x, 2));

        VectorHomo3d leftPistonJoint = VectorHomo3d(0, jointY, crankshaftAxis.z);
        leftPistonJoint = Rotate(leftPistonJoint, DegToRad(leftChamberAng));

        float connectingRodAng = GetAngleWithAxis(leftPistonJoint - rotatingPoint);

        leftPiston->Reposition(leftPistonJoint, true);

        leftChamber->GlobalRotate(0, 0, leftChamberAng, crankshaftAxis);

        leftConnectingRod->Reposition(rotatingPoint, true);
        leftConnectingRod->LocalRotate(0, 0, connectingRodAng, false);
    }

    void RightPart()
    {
        VectorHomo3d correctedRotationPoint = Rotate(rotatingPoint, DegToRad(-1 * rightChamberAng));        
        float jointY = correctedRotationPoint.y + sqrt(pow(connectingRodLength - pistonHeight * 0.5, 2) - pow(correctedRotationPoint.x, 2));

        VectorHomo3d rightPistonJoint = VectorHomo3d(0, jointY, crankshaftAxis.z);
        rightPistonJoint = Rotate(rightPistonJoint, DegToRad(rightChamberAng));

        float connectingRodAng = GetAngleWithAxis(rightPistonJoint - rotatingPoint);

        rightPiston->Reposition(rightPistonJoint, true);

        rightChamber->GlobalRotate(0, 0, rightChamberAng, crankshaftAxis);

        rightConnectingRod->Reposition(rotatingPoint, true);
        rightConnectingRod->LocalRotate(0, 0, connectingRodAng, false);
    }

    // Gira a manivela
    void SpinCrankshaft()
    {
        crankshaftAng += (1.5 * FpsController::getInstance().GetDeltaTime());
        crankshaftAng = crankshaftAng > PI_2 ? 0 : crankshaftAng;
        rotatingPoint = crankshaftAxis + VectorHomo3d(crankShaftAxisRadius * 2 * cos(crankshaftAng), crankShaftAxisRadius * 2 * sin(crankshaftAng), 0);
        crankPin->Reposition(rotatingPoint + crankPinOffset, true);

        backCounterWeight->LocalRotate(0, 0, crankshaftAng, false, crankshaftAxis);
        frontCounterWeight->LocalRotate(0, 0, crankshaftAng, false, crankshaftAxis);

        backMainJournal->LocalRotate(0, 0, crankshaftAng, false);
        frontMainJournal->LocalRotate(0, 0, crankshaftAng, false);
    }
};

#endif
