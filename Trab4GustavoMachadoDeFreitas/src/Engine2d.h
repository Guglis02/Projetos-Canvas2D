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
    Engine2d()
    {
        transformationMatrix = new Matrix3d();
    }
    ~Engine2d()
    {

    }

    void Render()
    {
        RenderCrankshaft();

        CV::color(0, 0, 1);

        vector<VectorHomo3d> chamber = {
            VectorHomo3d(-50, 200, 0),
            VectorHomo3d(-50, 300, 0),
            VectorHomo3d(50, 300, 0),
            VectorHomo3d(50, 200, 0)
        };

        // Calculates the piston joint
        chamberBase = (chamber[0] + chamber[2]) * 0.5f;
        VectorHomo3d dir = chamberBase - rotatingPoint;
        dir.normalize();

        CV::polygon(chamber);

        pistonJoint = rotatingPoint + (dir * l);
        // Draws the Connecting Rod
        CV::line(rotatingPoint, pistonJoint);
        // Draws the piston
        CV::line(pistonJoint.x - 50, pistonJoint.y, pistonJoint.x + 50, pistonJoint.y);
    }
private:
    Matrix3d* transformationMatrix;

    VectorHomo3d rotatingPoint;
    VectorHomo3d chamberBase;
    VectorHomo3d pistonJoint = VectorHomo3d(0, 200, 0);

    float ang = 0;
    int r = 30;
    int l = 240;

    VectorHomo3d crankshaftAxis = VectorHomo3d(0, 0, 0);

    // Desenha o virabrequim
    void RenderCrankshaft()
    {
        CV::color(1, 0, 0);
        CV::circle(crankshaftAxis, r, 32);

        ang += (2 * FpsController::getInstance().GetDeltaTime());
        ang = ang > PI_2 ? 0 : ang;
        rotatingPoint = crankshaftAxis + VectorHomo3d(r * 2 * cos(ang), r * 2 * sin(ang), 0);
        CV::circle(rotatingPoint, r, 32);

        // Desenha o contrapeso
        vector<VectorHomo3d> trapezium = {
            VectorHomo3d(rotatingPoint.x + r, rotatingPoint.y + r, 0),
            VectorHomo3d(rotatingPoint.x + r, rotatingPoint.y - r, 0),
            VectorHomo3d(rotatingPoint.x - (5 * r), rotatingPoint.y - (3 * r), 0),
            VectorHomo3d(rotatingPoint.x - (5 * r), rotatingPoint.y + (3 * r), 0)
        };

        transformationMatrix->Reset();
        transformationMatrix->Translation(rotatingPoint);
        transformationMatrix->RotationZ(ang);
        transformationMatrix->Translation(rotatingPoint * -1);

        CV::polygon(transformationMatrix->ApplyToPoints(trapezium));
    }
};

#endif
