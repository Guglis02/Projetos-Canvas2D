#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <iostream>

#include "gl_canvas2d.h"
#include "./Engines/Engine2d.h"
#include "./Engines/Engine3d.h"
#include "./Utils/GlobalConsts.h"
#include "./Models/Cube.h"
#include "./Models/Cilinder.h"
#include "./Models/Sphere.h"
#include "./Utils/FpsController.h"

using namespace std;

// Largura e altura inicial da tela. Alteram com o redimensionamento de tela.
int screenWidth = ConstScreenWidth, screenHeight = ConstScreenHeight;

Engine2d* engine2d;
vector<Engine3d*> engines3d;

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
            for (auto engine : engines3d)
            {
                engine->RenderOrtho(anglex, angley, anglez);
            }
            break;
        case viewMode::_perspective:
            for (auto engine : engines3d)
            {
                engine->RenderPersp(anglex, angley, anglez, d);
            }
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

    if(key == 'x')
        d--;
    if(key == 'z')
        d++;

    if(key == 'j')
        currentViewMode = viewMode::_2d;
    if(key == 'k')
        currentViewMode = viewMode::_ortho;
    if(key == 'l')
        currentViewMode = viewMode::_perspective;

    if(key == 'w')
        anglex++;
    if(key == 's')
        anglex--;
    if(key == 'a')
        angley++;
    if(key == 'd')
        angley--;
    if(key == 'q')
        anglez++;
    if(key == 'e')
        anglez--;
}

// Funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{

}

int main(void)
{
    engine2d = new Engine2d(VectorHomo3d(0, 0, 0));

    Engine3d* first3dEngine = new Engine3d(VectorHomo3d(0, 0, 250));
    engines3d.push_back(first3dEngine);

    CV::init(&screenWidth, &screenHeight, "Trabalho 4 - Gustavo Machado de Freitas");
    CV::run();
}
