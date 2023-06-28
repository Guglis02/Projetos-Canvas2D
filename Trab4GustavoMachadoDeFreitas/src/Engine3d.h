#ifndef __ENGINE3D_H__
#define __ENGINE3D_H__

#include <vector>
#include "./Utils/VectorHomo3d.h"
#include "./Utils/Matrix3d.h"
#include "./Utils/FpsController.h"
#include "./Utils/PointsUtils.h"
#include "./Models/Cube.h"
#include "./Models/Cilinder.h"
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

        //chamber = new Cube(VectorHomo3d(150, 200, 1), 100);
        chamber->LocalRotate(0, 0, 60 * invertionCoef, true);
        parts.push_back(chamber);

        rotatingPoint = crankshaftAxis + VectorHomo3d(crankShaftAxisRadius * 2 * cos(ang), crankShaftAxisRadius * 2 * sin(ang), 0);
        crankPin = new Cilinder(rotatingPoint, crankShaftAxisRadius, crankShaftAxisRadius, 20);
        crankPin->LocalRotate(90, 0, 0, true);
        parts.push_back(crankPin);

        mainJournal = new Cilinder(crankshaftAxis, crankShaftAxisRadius, crankShaftAxisRadius, 20);
        mainJournal->LocalRotate(90, 0, 0, true);
        parts.push_back(mainJournal);
    }
    ~Engine3d()
    {

    }

    void Render(float anglex, float angley, float anglez)
    {
        for (auto part : parts)
        {
            part->GlobalRotate(anglex, angley, anglez);
        }

        SpinCrankshaft();
        RenderCrankshaft();

        CV::color(0, 0, 1);

        chamber->Draw();

        // Calculates the piston joint
        chamberBase = chamber->GetCenter();
        VectorHomo3d dir = chamberBase - rotatingPoint;
        dir.normalize();

        pistonJoint = rotatingPoint + (dir * connectingRodLength);
        // Draws the Connecting Rod
        CV::line(rotatingPoint, pistonJoint);

        //DrawPiston();
    }
private:
    Matrix3d* transformationMatrix;

    vector<Model*> parts;
    Cube* chamber;
    Cilinder* mainJournal;
    Cilinder* crankPin;

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
        crankPin->Reposition(rotatingPoint, false);
        mainJournal->LocalRotate(0, 0, ang, false);
    }

    // Desenha o virabrequim
    void RenderCrankshaft()
    {
        CV::color(1, 0, 0);

        mainJournal->Draw();
        crankPin->Draw();

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
