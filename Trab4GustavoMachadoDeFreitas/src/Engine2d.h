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

        chamber = {
            crankshaftAxis + VectorHomo3d((invertionCoef * 200), 150, 0),
            crankshaftAxis + VectorHomo3d((invertionCoef * 200), 250, 0),
            crankshaftAxis + VectorHomo3d((invertionCoef * 100), 250, 0),
            crankshaftAxis + VectorHomo3d((invertionCoef * 100), 150, 0)
        };

        transformationMatrix->Reset();
        transformationMatrix->Translation(GetCenter(chamber));
        transformationMatrix->RotationZ(DegToRad(60) * invertionCoef);
        transformationMatrix->Translation(GetCenter(chamber) * -1);
        chamber = transformationMatrix->ApplyToPoints(chamber);

        // Calculates the piston joint
        chamberBase = (chamber[0] + chamber[2]) * 0.5f;
        VectorHomo3d dir = chamberBase - rotatingPoint;
        dir.normalize();

        CV::polygon(chamber);

        pistonJoint = rotatingPoint + (dir * connectingRodLength);
        // Draws the Connecting Rod
        CV::line(rotatingPoint, pistonJoint);

        DrawPiston();
        // // Draws the piston
        // CV::line(pistonJoint.x - crankShaftAxisRadius, pistonJoint.y, pistonJoint.x + crankShaftAxisRadius, pistonJoint.y);
    }
private:
    Matrix3d* transformationMatrix;

    vector<VectorHomo3d> chamber;

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

    void DrawPiston()
    {
        // Calcula linha oposta ao pistão
        VectorHomo3d chamberDir = chamber[1] - chamber[0];
        chamberDir.normalize();    
        
        VectorHomo3d chamberMiddle = (chamber[0] + chamber[1]) * 0.5f;
        
        // Linha perpendicular ao fundo da câmara
        VectorHomo3d perpDir = VectorHomo3d(-chamberDir.y, chamberDir.x, 0);

        // Calcula a projeção do pistão na câmara
        VectorHomo3d pistonProj = pistonJoint - chamberMiddle;
        float projLength = pistonProj * (perpDir);
        VectorHomo3d pistonProjDir = perpDir * projLength;
        VectorHomo3d pistonProjPoint = chamberMiddle + pistonProjDir;

        // Calcula as pontas do pistão
        VectorHomo3d pistonStart = pistonProjPoint - (chamberDir * 50);
        VectorHomo3d pistonEnd = pistonProjPoint + (chamberDir * 50);

        // Desenha o pistão
        CV::line(pistonStart, pistonEnd);
    }
};

#endif
