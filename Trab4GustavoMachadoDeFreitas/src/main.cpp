#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <iostream>

#include "gl_canvas2d.h"
#include "./Utils/GlobalConsts.h"
#include "./Models/Cube.h"

using namespace std;

// Largura e altura inicial da tela. Alteram com o redimensionamento de tela.
int screenWidth = ConstScreenWidth, screenHeight = ConstScreenHeight;

Cube* cube;

int d = 50;
int anglex = 0;
int angley = 0;
int anglez = 0;

void render(void)
{
    CV::color(1, 0, 0);
    CV::translate(screenWidth >> 1, screenHeight >> 1);
    cube->Transform(anglex, angley, anglez);
    cube->Draw();
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
    cube = new Cube();

    CV::init(&screenWidth, &screenHeight, "Trabalho 4 - Gustavo Machado de Freitas");
    CV::run();
}
