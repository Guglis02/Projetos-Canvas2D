#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <stdio.h>
#include <cstdlib>
#include <vector>

#include "gl_canvas2d.h"
#include "./Utils/GlobalConsts.h"
#include "./Utils/VectorHomo3d.h"

#include <iostream>
#define augusto 3

using namespace std;

// Largura e altura inicial da tela. Alteram com o redimensionamento de tela.
int screenWidth = ConstScreenWidth, screenHeight = ConstScreenHeight;

vector<VectorHomo3d*> cubo;
int z1 = 1, z2 = 2, d =1;
int anglex = 0;
void render(void)
{
    CV::color(1, 0, 0);
    CV::translate(screenWidth >> 1, screenHeight >> 1);

    for (unsigned int i = 0; i < cubo.size(); i++)
    {
        for (unsigned int j = 0; j < cubo.size(); j++)
        {
            int counter = 0;

            if (cubo[i]->x == cubo[j]->x)
            {
                counter++;
            }
            if (cubo[i]->y == cubo[j]->y)
            {
                counter++;
            }
            if (cubo[i]->z == cubo[j]->z)
            {
                counter++;
            }

            if (counter == 2)
            {
                CV::line(cubo[i]->x*d / cubo[i]->z,
                         cubo[i]->y*d / cubo[i]->z,
                         cubo[j]->x*d / cubo[j]->z,
                         cubo[j]->y*d / cubo[j]->z);
            }
        }
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
    cout << key;
    if(key == 200)
        z2++;
    if(key == 202)
        z2--;
    if(key == 201){
        z1++;
        z2++;
    }

    if(key == 203){
        z1--;
        z2--;
    }
    if(key == 'd')
        d--;
    if(key == 'f')
        d++;

    if(key == 52)
        anglex++;
    if(key == 54)
        anglex--;
    cubo.clear();
    cubo.push_back(new VectorHomo3d(-250, -250, z1));
    cubo.push_back(new VectorHomo3d(-250, -250, z2));
    cubo.push_back(new VectorHomo3d(-250, 250, z1));
    cubo.push_back(new VectorHomo3d(-250, 250, z2));
    cubo.push_back(new VectorHomo3d(250, -250, z1));
    cubo.push_back(new VectorHomo3d(250, -250, z2));
    cubo.push_back(new VectorHomo3d(250, 250, z1));
    cubo.push_back(new VectorHomo3d(250, 250, z2));
}

// Funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{

}

int main(void)
{
    cubo.push_back(new VectorHomo3d(-250, -250, z1));
    cubo.push_back(new VectorHomo3d(-250, -250, z2));
    cubo.push_back(new VectorHomo3d(-250, 250, z1));
    cubo.push_back(new VectorHomo3d(-250, 250, z2));
    cubo.push_back(new VectorHomo3d(250, -250, z1));
    cubo.push_back(new VectorHomo3d(250, -250, z2));
    cubo.push_back(new VectorHomo3d(250, 250, z1));
    cubo.push_back(new VectorHomo3d(250, 250, z2));

    CV::init(&screenWidth, &screenHeight, "Trabalho 4 - Gustavo Machado de Freitas");
    CV::run();
}
