#ifndef __ENGINE2D_H__
#define __ENGINE2D_H__

#include <vector>
#include "../Utils/VectorHomo3d.h"
#include "../Utils/Matrix3d.h"
#include "../Utils/FpsController.h"
#include "../Utils/PointsUtils.h"
#include "./Engine.h"
#include "../gl_canvas2d.h"

using namespace std;

class Engine2d : public Engine
{
public:
    Engine2d(VectorHomo3d crankshaftAxis) : Engine(crankshaftAxis)
    {
        this->chamber = {
            crankshaftAxis + VectorHomo3d(-60, 200, 0),
            crankshaftAxis + VectorHomo3d(-60, 320, 0),
            crankshaftAxis + VectorHomo3d(60, 320, 0),
            crankshaftAxis + VectorHomo3d(60, 200, 0)
        };
    }
    ~Engine2d()
    {

    }

    void Render()
    {
        SpinCrankshaft();
        if (isShowingCrankshaft)
        {
            RenderCrankshaft();
        }

        LeftPart();
        RightPart();
    }
protected:
    vector<VectorHomo3d> chamber;

    VectorHomo3d rotatingPoint;
    VectorHomo3d chamberBase;
    VectorHomo3d pistonJoint = VectorHomo3d(0, 200, 0);

    int crankShaftAxisRadius = 30;
    int connectingRodLength = 260;

    void SpinCrankshaft()
    {
        float crankshaftAngIncrement = (rpm / 60.0f) * 2.0f * PI * FpsController::getInstance().GetDeltaTime();
        crankshaftAng += crankshaftAngIncrement;
        crankshaftAng = crankshaftAng > PI_2 ? 0 : crankshaftAng;
        rotatingPoint = crankshaftAxis + VectorHomo3d(crankShaftAxisRadius * 2 * cos(crankshaftAng), crankShaftAxisRadius * 2 * sin(crankshaftAng), 0);
    }

    // Desenha o virabrequim
    void RenderCrankshaft()
    {        
        CV::color(1, 0, 0);
        CV::circle(crankshaftAxis, crankShaftAxisRadius, 32);
        CV::circle(rotatingPoint, crankShaftAxisRadius, 32);

        // Desenha o contrapeso
        vector<VectorHomo3d> counterWeight = {
            VectorHomo3d(rotatingPoint.x + crankShaftAxisRadius, rotatingPoint.y + crankShaftAxisRadius, 0),
            VectorHomo3d(rotatingPoint.x + crankShaftAxisRadius, rotatingPoint.y - crankShaftAxisRadius, 0),
            VectorHomo3d(rotatingPoint.x - (5 * crankShaftAxisRadius), rotatingPoint.y - (3 * crankShaftAxisRadius), 0),
            VectorHomo3d(rotatingPoint.x - (5 * crankShaftAxisRadius), rotatingPoint.y + (3 * crankShaftAxisRadius), 0)
        };

        transformationMatrix->Reset();
        transformationMatrix->Translation(rotatingPoint);
        transformationMatrix->RotationZ(crankshaftAng);
        transformationMatrix->Translation(rotatingPoint * -1);

        CV::polygon(transformationMatrix->ApplyToPoints(counterWeight));
    }

    void LeftPart()
    {
        VectorHomo3d correctedRotationPoint = Rotate(rotatingPoint, DegToRad(-1 * leftChamberAng));
        float jointY = correctedRotationPoint.y + sqrt(pow(connectingRodLength, 2) - pow(correctedRotationPoint.x, 2));

        pistonJoint = VectorHomo3d(0, jointY, 0);
        VectorHomo3d pistonSide1 = pistonJoint + VectorHomo3d(-60, 0, 0);
        VectorHomo3d pistonSide2 = pistonJoint + VectorHomo3d(60, 0, 0);

        pistonJoint = Rotate(pistonJoint, DegToRad(leftChamberAng));
        pistonSide1 = Rotate(pistonSide1, DegToRad(leftChamberAng));
        pistonSide2 = Rotate(pistonSide2, DegToRad(leftChamberAng));

        transformationMatrix->Reset();
        transformationMatrix->RotationZ(DegToRad(leftChamberAng));
        vector<VectorHomo3d> transformedChamber = transformationMatrix->ApplyToPoints(chamber);

        if (isShowingChamber)
        {
            CV::color(0, 1, 0);
            CV::polygon(transformedChamber);
        }

        if (isShowingPiston)
        {
            CV::color(0, 0, 1);
            CV::line(rotatingPoint, pistonJoint);
            CV::line(pistonSide1, pistonSide2);
        }
    }

    void RightPart()
    {
        VectorHomo3d correctedRotationPoint = Rotate(rotatingPoint, DegToRad(-1 * rightChamberAng));
        float jointY = correctedRotationPoint.y + sqrt(pow(connectingRodLength, 2) - pow(correctedRotationPoint.x, 2));

        pistonJoint = VectorHomo3d(0, jointY, 0);
        VectorHomo3d pistonSide1 = pistonJoint + VectorHomo3d(-60, 0, 0);
        VectorHomo3d pistonSide2 = pistonJoint + VectorHomo3d(60, 0, 0);

        pistonJoint = Rotate(pistonJoint, DegToRad(rightChamberAng));
        pistonSide1 = Rotate(pistonSide1, DegToRad(rightChamberAng));
        pistonSide2 = Rotate(pistonSide2, DegToRad(rightChamberAng));

        transformationMatrix->Reset();
        transformationMatrix->RotationZ(DegToRad(rightChamberAng));
        vector<VectorHomo3d> transformedChamber = transformationMatrix->ApplyToPoints(chamber);

        if (isShowingChamber)
        {
            CV::color(0, 1, 0);
            CV::polygon(transformedChamber);
        }

        if (isShowingPiston)
        {
            CV::color(0, 0, 1);
            CV::line(rotatingPoint, pistonJoint);
            CV::line(pistonSide1, pistonSide2);
        }
    }
};

#endif
