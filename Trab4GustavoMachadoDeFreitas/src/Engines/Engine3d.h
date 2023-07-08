#ifndef __ENGINE3D_H__
#define __ENGINE3D_H__

#include <vector>
#include "../Utils/VectorHomo3d.h"
#include "../Utils/Matrix3d.h"
#include "../Utils/FpsController.h"
#include "../Utils/PointsUtils.h"
#include "../Models/Cube.h"
#include "../Models/Cilinder.h"
#include "../Models/CounterWeight.h"
#include "../Models/Model.h"
#include "../gl_canvas2d.h"

using namespace std;

class Engine3d : public Engine
{
public:
    Engine3d(VectorHomo3d crankshaftAxis) : Engine(crankshaftAxis)
    {
        chamberBase = crankshaftAxis + VectorHomo3d(0, 250, 0);
        leftChamber = new Cube(chamberBase, 4, 150, 150);
        parts.push_back(leftChamber);
        chamberParts.push_back(leftChamber);

        rightChamber = new Cube(chamberBase, 4, 150, 150);
        parts.push_back(rightChamber);
        chamberParts.push_back(rightChamber);

        rotatingPoint = crankshaftAxis + VectorHomo3d(crankShaftAxisRadius * 2 * cos(crankshaftAng), crankShaftAxisRadius * 2 * sin(crankshaftAng), 0);
        crankPin = new Cilinder(rotatingPoint + crankPinOffset, 30, 55, crankShaftAxisRadius);
        parts.push_back(crankPin);
        crankshaftParts.push_back(crankPin);

        backMainJournal = new Cilinder(crankshaftAxis + VectorHomo3d(0, 0, 15), 20, 150, crankShaftAxisRadius);
        parts.push_back(backMainJournal);
        crankshaftParts.push_back(backMainJournal);

        frontMainJournal = new Cilinder(crankshaftAxis + VectorHomo3d(0, 0, -15), 20, 150, crankShaftAxisRadius);
        frontMainJournal->LocalRotate(0, DegToRad(180), 0, true);
        parts.push_back(frontMainJournal);
        crankshaftParts.push_back(frontMainJournal);

        backCounterWeight = new CounterWeight(crankshaftAxis + VectorHomo3d(0, 0, 15), 4, 20, 8 * crankShaftAxisRadius, 3 * crankShaftAxisRadius);
        parts.push_back(backCounterWeight);
        crankshaftParts.push_back(backCounterWeight);

        frontCounterWeight = new CounterWeight(crankshaftAxis + VectorHomo3d(0, 0, -25), 4, 20, 8 * crankShaftAxisRadius, 3 * crankShaftAxisRadius);
        parts.push_back(frontCounterWeight);
        crankshaftParts.push_back(frontCounterWeight);

        leftConnectingRod = new Cilinder(rotatingPoint, 20, connectingRodLength, 5);
        leftConnectingRod->LocalRotate(0, DegToRad(90), 0, true);
        parts.push_back(leftConnectingRod);
        pistonParts.push_back(leftConnectingRod);

        rightConnectingRod = new Cilinder(rotatingPoint, 20, connectingRodLength, 5);
        rightConnectingRod->LocalRotate(0, DegToRad(90), 0, true);
        parts.push_back(rightConnectingRod);
        pistonParts.push_back(rightConnectingRod);

        leftPiston = new Cube(chamberBase, 4, 150, pistonHeight);
        parts.push_back(leftPiston);
        pistonParts.push_back(leftPiston);

        rightPiston = new Cube(chamberBase, 4, 150, pistonHeight);
        parts.push_back(rightPiston);
        pistonParts.push_back(rightPiston);
    }
    ~Engine3d()
    {
    }

    void Render(float anglex, float angley, float anglez, int d, bool shouldDrawPerspective)
    {
        Update(anglex, angley, anglez);

        if (isShowingChamber)
        {
            CV::color(0, 1, 0);
            for (auto part : chamberParts)
            {
                shouldDrawPerspective ? part->DrawPerspective(d) : part->DrawOrthogonal();
            }
        }

        if (isShowingPiston)
        {
            CV::color(0, 0, 1);
            for (auto part : pistonParts)
            {
                shouldDrawPerspective ? part->DrawPerspective(d) : part->DrawOrthogonal();
            }
        }

        if (isShowingCrankshaft)
        {
            CV::color(1, 0, 0);
            for (auto part : crankshaftParts)
            {
                shouldDrawPerspective ? part->DrawPerspective(d) : part->DrawOrthogonal();
            }
        }
    }

private:
    // Peças do motor
    vector<Model *> parts;
    vector<Model *> chamberParts;
    vector<Model *> pistonParts;
    vector<Model *> crankshaftParts;
    Cube *leftChamber;
    Cube *rightChamber;
    Cube *leftPiston;
    Cube *rightPiston;
    Cilinder *backMainJournal;
    Cilinder *frontMainJournal;
    Cilinder *crankPin;
    Cilinder *leftConnectingRod;
    Cilinder *rightConnectingRod;
    CounterWeight *frontCounterWeight;
    CounterWeight *backCounterWeight;

    // Pontos importantes para o calculo do pistão
    VectorHomo3d rotatingPoint;
    VectorHomo3d chamberBase;
    VectorHomo3d pistonJoint = VectorHomo3d(0, 200, 0);
    VectorHomo3d crankPinOffset = VectorHomo3d(0, 0, -15);

    int crankShaftAxisRadius = 30;
    int connectingRodLength = 290;
    int pistonHeight = 40;

    void UpdatePiston(Cube *chamber, Cube *piston, Cilinder *connectingRod, float chamberAng)
    {
        VectorHomo3d correctedRotationPoint = Rotate(rotatingPoint, DegToRad(-1 * chamberAng));
        float jointY = correctedRotationPoint.y + sqrt(pow(connectingRodLength - pistonHeight * 0.5, 2) - pow(correctedRotationPoint.x, 2));

        VectorHomo3d pistonJoint = VectorHomo3d(0, jointY, crankshaftAxis.z);
        pistonJoint = Rotate(pistonJoint, DegToRad(chamberAng));

        float connectingRodAng = GetAngleWithAxis(pistonJoint - rotatingPoint);

        piston->Reposition(pistonJoint, true);
        piston->LocalRotate(0, 0, DegToRad(chamberAng), false);

        chamber->GlobalRotate(0, 0, chamberAng);

        connectingRod->Reposition(rotatingPoint, true);
        connectingRod->LocalRotate(0, 0, connectingRodAng, false);
    }

    void SpinCrankshaft()
    {
        float crankshaftAngIncrement = (rpm / 60.0f) * 2.0f * PI * FpsController::getInstance().GetDeltaTime();
        crankshaftAng += crankshaftAngIncrement;
        crankshaftAng = crankshaftAng > PI_2 ? 0 : crankshaftAng;
        rotatingPoint = crankshaftAxis + VectorHomo3d(crankShaftAxisRadius * 2 * cos(crankshaftAng), crankShaftAxisRadius * 2 * sin(crankshaftAng), 0);
        crankPin->Reposition(rotatingPoint + crankPinOffset, true);

        backCounterWeight->LocalRotate(0, 0, crankshaftAng, false, crankshaftAxis);
        frontCounterWeight->LocalRotate(0, 0, crankshaftAng, false, crankshaftAxis);

        backMainJournal->LocalRotate(0, 0, crankshaftAng, false);
        frontMainJournal->LocalRotate(0, 0, crankshaftAng, false);
    }

    // Atualiza peças do motor
    void Update(float anglex, float angley, float anglez)
    {
        for (auto part : parts)
        {
            part->ResetTransformedPoints();
        }

        SpinCrankshaft();
        UpdatePiston(leftChamber, leftPiston, leftConnectingRod, leftChamberAng);
        UpdatePiston(rightChamber, rightPiston, rightConnectingRod, rightChamberAng);

        for (auto part : parts)
        {
            part->GlobalRotate(anglex, angley, anglez);
        }
    }
};

#endif
