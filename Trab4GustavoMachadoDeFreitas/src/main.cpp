#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <iostream>

#include "gl_canvas2d.h"
#include "./Utils/GlobalConsts.h"
#include "./Utils/VectorHomo3d.h"
#include "./Utils/Matrix3d.h"

using namespace std;

// Largura e altura inicial da tela. Alteram com o redimensionamento de tela.
int screenWidth = ConstScreenWidth, screenHeight = ConstScreenHeight;

vector<VectorHomo3d*> cubeModel;
vector<VectorHomo3d*> transformedCube;
Matrix3d* transformationMatrix;

int d =1;
int anglex = 0;
int angley = 0;
int anglez = 0;

void TransformCube()
{
    transformedCube.clear();
    transformationMatrix->Reset();
    transformationMatrix->RotationX(anglex * PI / 180.0f);
    transformationMatrix->RotationY(angley * PI / 180.0f);
    transformationMatrix->RotationZ(anglez * PI / 180.0f);
    transformedCube = transformationMatrix->ApplyToPoints(cubeModel);
}

void CubeProjection(vector<VectorHomo3d*>& points, int d)
{
    for (unsigned int i = 0; i < points.size(); i++)
    {
        float z = points[i]->z;
        if (z != 0) {
            points[i]->x = points[i]->x * d / z;
            points[i]->y = points[i]->y * d / z;
        }
    }
}

void RenderCube()
{
    CV::color(1, 0, 0);
    for (unsigned int i = 0; i < transformedCube.size(); i+=2)
    {
        CV::circle(*transformedCube[i], 10, 10);
    }

    CV::line(*transformedCube[0], *transformedCube[1]);
    CV::line(*transformedCube[1], *transformedCube[2]);
    CV::line(*transformedCube[2], *transformedCube[3]);
    CV::line(*transformedCube[3], *transformedCube[0]);

    CV::line(*transformedCube[4], *transformedCube[5]);
    CV::line(*transformedCube[5], *transformedCube[6]);
    CV::line(*transformedCube[6], *transformedCube[7]);
    CV::line(*transformedCube[7], *transformedCube[4]);

    CV::line(*transformedCube[0], *transformedCube[4]);
    CV::line(*transformedCube[1], *transformedCube[5]);
    CV::line(*transformedCube[2], *transformedCube[6]);
    CV::line(*transformedCube[3], *transformedCube[7]);
}

void render(void)
{
    CV::color(1, 0, 0);
    CV::translate(screenWidth >> 1, screenHeight >> 1);
    TransformCube();
    //CubeProjection(transformedCube, d);
    RenderCube();
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

    if(key == 'd')
        d--;
    if(key == 'f')
        d++;

    if(key == 52) // 4 aumenta angulo x
        anglex++;
    if(key == 54) // 6 diminui angulo x
        anglex--;
    if(key == 56) // 8 aumenta angulo y
        angley++;
    if(key == 50) // 2 diminui angulo y
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
    cubeModel.push_back(new VectorHomo3d(-250, -250, -250));
    cubeModel.push_back(new VectorHomo3d(-250, -250, 250));
    cubeModel.push_back(new VectorHomo3d(-250, 250, 250));
    cubeModel.push_back(new VectorHomo3d(-250, 250, -250));
    cubeModel.push_back(new VectorHomo3d(250, -250, -250));
    cubeModel.push_back(new VectorHomo3d(250, -250, 250));
    cubeModel.push_back(new VectorHomo3d(250, 250, 250));
    cubeModel.push_back(new VectorHomo3d(250, 250, -250));

    transformationMatrix = new Matrix3d();

    CV::init(&screenWidth, &screenHeight, "Trabalho 4 - Gustavo Machado de Freitas");
    CV::run();
}
