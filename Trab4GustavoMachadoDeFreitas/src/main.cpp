#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <iostream>

#include "gl_canvas2d.h"
#include "Engine2d.h"
// #include "Engine3d.h"
#include "./Utils/GlobalConsts.h"
#include "./Models/Cube.h"
#include "./Models/Cilinder.h"
#include "./Models/Sphere.h"
#include "./Utils/FpsController.h"

using namespace std;

// Largura e altura inicial da tela. Alteram com o redimensionamento de tela.
int screenWidth = ConstScreenWidth, screenHeight = ConstScreenHeight;

Engine2d* first2dEngine;
Engine2d* second2dEngine;
// Engine3d* first3dEngine;
// Engine3d* second3dEngine;

int d = 200;
int anglex = 0;
int angley = 0;
int anglez = 0;

const int NM = 20;
Sphere* sphere;
Cube* cube;
Cilinder* cilinder;

void render(void)
{
    FpsController::getInstance().updateFrames();
    CV::color(1, 0, 0);
    CV::translate(screenWidth >> 1, screenHeight >> 1);

    // first3dEngine->Render(anglex, angley, anglez);

    // sphere->LocalRotate(anglex, angley, anglez, false);
    // sphere->Draw(d);

    // cilinder->LocalRotate(anglex, angley, anglez, false);
    // cilinder->DrawPerspective(d);
    // cilinder->DrawOrthogonal();

    cube->LocalRotate(anglex, angley, anglez, false);
    cube->DrawPerspective(d);

    // first2dEngine->Render();
    // second2dEngine->Render();
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
    first2dEngine = new Engine2d(VectorHomo3d(0, 0, 0), false, DegToRad(0));
    second2dEngine = new Engine2d(VectorHomo3d(0, 0, 0), true, DegToRad(180));

    // first3dEngine = new Engine3d(VectorHomo3d(0, 0, 0), false, DegToRad(0));

    sphere = new Sphere(VectorHomo3d(0, 0, 200), NM, 100);
    cilinder = new Cilinder(VectorHomo3d(0, 0, 200), NM, 100, 100);
    cube = new Cube(VectorHomo3d(0, 0, 200), 4, 100);

    CV::init(&screenWidth, &screenHeight, "Trabalho 4 - Gustavo Machado de Freitas");
    CV::run();
}
