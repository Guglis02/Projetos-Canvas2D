#ifndef __ENGINE2D_H__
#define __ENGINE2D_H__

#include <vector>
#include "./Utils/VectorHomo3d.h"
#include "./Utils/Matrix3d.h"
#include "./Utils/FpsController.h"
#include "./Utils/PointsUtils.h"
#include "gl_canvas2d.h"

using namespace std;

class Engine2d
{
public:
    Engine2d(VectorHomo3d crankshaftAxis, bool isFlipped, float startAngle = 0)
    {
        this->crankshaftAxis = crankshaftAxis;
        this->invertionCoef = isFlipped ? -1 : 1;
        this->ang = startAngle;
        transformationMatrix = new Matrix3d();
    }
    ~Engine2d()
    {

    }

    void Render()
    {
        SpinCrankshaft();
        RenderCrankshaft();

        CV::color(0, 0, 1);

        vector<VectorHomo3d> chamber = {
            crankshaftAxis + VectorHomo3d((invertionCoef * 125), 200, 0),
            crankshaftAxis + VectorHomo3d((invertionCoef * 125), 300, 0),
            crankshaftAxis + VectorHomo3d((invertionCoef * 25), 300, 0),
            crankshaftAxis + VectorHomo3d((invertionCoef * 25), 200, 0)
        };

        // Calculates the piston joint
        chamberBase = (chamber[0] + chamber[2]) * 0.5f;
        VectorHomo3d dir = chamberBase - rotatingPoint;
        dir.normalize();

        CV::polygon(chamber);

        pistonJoint = rotatingPoint + (dir * connectingRodLength);
        // Draws the Connecting Rod
        CV::line(rotatingPoint, pistonJoint);
        // Draws the piston
        CV::line(pistonJoint.x - crankShaftAxisRadius, pistonJoint.y, pistonJoint.x + crankShaftAxisRadius, pistonJoint.y);
    }
private:
    Matrix3d* transformationMatrix;

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
        ang += (2 * FpsController::getInstance().GetDeltaTime());
        ang = ang > PI_2 ? 0 : ang;
        rotatingPoint = crankshaftAxis + VectorHomo3d(crankShaftAxisRadius * 2 * cos(ang), crankShaftAxisRadius * 2 * sin(ang), 0);
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
        transformationMatrix->RotationZ(ang);
        transformationMatrix->Translation(rotatingPoint * -1);

        CV::polygon(transformationMatrix->ApplyToPoints(counterWeight));
    }
};

#endif
