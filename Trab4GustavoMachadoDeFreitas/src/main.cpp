#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <iostream>

#include "gl_canvas2d.h"
#include "Engine2d.h"
#include "Engine3d.h"
#include "./Utils/GlobalConsts.h"
#include "./Models/Cube.h"
#include "./Models/Cilinder.h"
#include "./Models/Sphere.h"
#include "./Utils/FpsController.h"

using namespace std;

// Largura e altura inicial da tela. Alteram com o redimensionamento de tela.
int screenWidth = ConstScreenWidth, screenHeight = ConstScreenHeight;

Engine2d* engine2d;
Engine3d* first3dEngine;
Engine3d* second3dEngine;

int d = 200;
int anglex = 0;
int angley = 0;
int anglez = 0;

enum class viewMode
{
    _2d,
    _ortho,
    _perspective
};

viewMode currentViewMode = viewMode::_2d;

void render(void)
{
    FpsController::getInstance().updateFrames();
    CV::color(1, 0, 0);
    CV::translate(screenWidth >> 1, screenHeight >> 1);

    anglex = anglex > 360 ? 0 : anglex;
    angley = angley > 360 ? 0 : angley;
    anglez = anglez > 360 ? 0 : anglez;

    switch (currentViewMode)
    {
        case viewMode::_2d:
            engine2d->Render();
            break;
        case viewMode::_ortho:
            first3dEngine->Render(anglex, angley, anglez, d);
            second3dEngine->Render(anglex, angley, anglez, d);
            break;
        case viewMode::_perspective:  
            first3dEngine->Render(anglex, angley, anglez, d);
            second3dEngine->Render(anglex, angley, anglez, d);
            break;
        default:
            break;
    }
}

// Funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    // printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);
}

// Funcao chamada toda vez que uma tecla for pressionada
void keyboard(int key)
{
    //cout << key;

    if(key == 'd')
        d--;
    if(key == 'f')
        d++;

    if(key == 56) // 8 aumenta angulo x
        anglex++;
    if(key == 50) // 2 diminui angulo x
        anglex--;
    if(key == 52) // 4 aumenta angulo y
        angley++;
    if(key == 54) // 6 diminui angulo y
        angley--;
    if(key == 55) // 7 aumenta angulo z
        anglez++;
    if(key == 57) // 9 diminui angulo z
        anglez--;
}

// Funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{

}

int main(void)
{
    engine2d = new Engine2d(VectorHomo3d(0, 0, 0));

    first3dEngine = new Engine3d(VectorHomo3d(0, 0, 0));
    second3dEngine = new Engine3d(VectorHomo3d(0, 0, 250));

    CV::init(&screenWidth, &screenHeight, "Trabalho 4 - Gustavo Machado de Freitas");
    CV::run();
}
