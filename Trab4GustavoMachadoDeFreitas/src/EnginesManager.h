#ifndef ENGINESMANAGER_H
#define ENGINESMANAGER_H

#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <math.h>

#include "gl_canvas2d.h"
#include "./Engines/Engine2d.h"
#include "./Engines/Engine3d.h"
#include "./Engines/Engine.h"
#include "./Models/Cube.h"
#include "./Models/Cilinder.h"
#include "./Utils/FpsController.h"
#include "./Utils/UIManager.h"
#include "./Utils/ViewMode.h"

using namespace std;

// Classe responsável por gerenciar todos os motores.
class EnginesManager
{
public:
    EnginesManager()
    {
        uiManager = new UIManager();
        engine2d = new Engine2d(VectorHomo3d(0, 0, 0));
        engines.push_back(engine2d);
        AddEngine();
    }

    void Render()
    {
        CV::translate(ConstScreenWidth >> 1, ConstScreenHeight >> 1);
        uiManager->Update(rpm, angleBetweenPistons, engines3d.size(), d, currentViewMode, currentVariable);

        anglex = anglex > 360 ? 0 : anglex;
        angley = angley > 360 ? 0 : angley;
        anglez = anglez > 360 ? 0 : anglez;

        for (auto engine : engines)
        {
            engine->SetRpm(rpm);
            engine->SetAngleBetweenPistons(angleBetweenPistons);
        }

        switch (currentViewMode)
        {
        case viewMode::_2d:
            engine2d->Render();
            break;
        case viewMode::_ortho:
            for (auto engine : engines3d)
            {
                engine->Render(anglex, angley, anglez, d, false);
            }
            break;
        case viewMode::_perspective:
            for (auto engine : engines3d)
            {
                engine->Render(anglex, angley, anglez, d, true);
            }
            break;
        default:
            break;
        }

        uiManager->Draw();
    }

    void ProcessKey(int key)
    {
        switch (key)
        {
        case '+':
            IncrementCurrentVariable(1);
            break;
        case '-':
            IncrementCurrentVariable(-1);
            break;
        case 'j':
            currentViewMode = viewMode::_2d;
            break;
        case 'k':
            currentViewMode = viewMode::_ortho;
            break;
        case 'l':
            currentViewMode = viewMode::_perspective;
            break;
        case 'w':
            anglex++;
            break;
        case 's':
            anglex--;
            break;
        case 'a':
            angley++;
            break;
        case 'd':
            angley--;
            break;
        case 'q':
            anglez++;
            break;
        case 'e':
            anglez--;
            break;
        case 'c':
            for (auto engine : engines)
            {
                engine->ToggleChamberView();
            }
            break;
        case 'p':
            for (auto engine : engines)
            {
                engine->TogglePistonView();
            }
            break;
        case 'v':
            for (auto engine : engines)
            {
                engine->ToggleCrankshaftView();
            }
            break;
        case '1':
        case '2':
        case '3':
        case '4':
            currentVariable = key - '0';
            break;
        default:
            break;
        }
    }

private:
    Engine2d *engine2d;
    vector<Engine3d *> engines3d;
    // Vetor usado em situações onde tanto o motor 2D quanto os motores 3D devem ser chamados
    vector<Engine *> engines;
    UIManager *uiManager;

    int engineZ = 0;
    int engineOffset = 250;

    int d = 200;
    // O Rpm pode ser negativado para inverter o sentido da rotação
    float rpm = 5;
    // O angulo entre os pistões é limitado entre 60 e 110 conforme a especificação
    int angleBetweenPistons = 90;

    int anglex = 0;
    int angley = 0;
    int anglez = 0;

    viewMode currentViewMode = viewMode::_2d;
    int currentVariable = 0;

    void AddEngine()
    {
        engineZ += engineOffset;
        engines3d.push_back(new Engine3d(VectorHomo3d(0, 0, engineZ)));
        engines.push_back(engines3d.back());
    }

    // Remove até sobrar no mínimo um motor 3d
    void RemoveEngine()
    {
        if (engines3d.size() > 1)
        {
            engineZ -= engineOffset;
            engines3d.pop_back();
            engines.pop_back();
        }
    }

    void IncrementCurrentVariable(int inc)
    {
        switch (currentVariable)
        {
        case 1:
            rpm += inc;
            break;
        case 2:
            angleBetweenPistons = max(60, min(angleBetweenPistons + inc, 110));
            break;
        case 3:
            inc > 0 ? AddEngine() : RemoveEngine();
            break;
        case 4:
            d += inc;
            break;
        default:
            break;
        }
    }
};

#endif