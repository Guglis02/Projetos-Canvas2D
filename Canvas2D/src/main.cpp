/*********************************************************************
// Canvas2D
// Autor: Cesar Tadeu Pozzer
//        04/2021
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include "gl_canvas2d.h"


int mx, my; //coordenadas do mouse

int blockSize = 100;
int halfBlockSize = blockSize / 2;
int quarterBlockSize = blockSize / 4;

void renderIsometricCube(float x, float y, int testeX, int testeY)
{
    color(15);

    float topx[] = {x, x + halfBlockSize, x, x - halfBlockSize};
    float topy[] = {y - halfBlockSize, y - quarterBlockSize, y, y - quarterBlockSize};

    polygonFill(topx, topy, 4);

    color(14);

    float frontx[] = {x, x + halfBlockSize, x + halfBlockSize, x};
    float fronty[] = {y, y - quarterBlockSize, y + quarterBlockSize, y + halfBlockSize};

    polygonFill(frontx, fronty, 4);

    color(16);

    float backx[] = {x, x - halfBlockSize, x - halfBlockSize, x};
    float backy[] = {y, y - quarterBlockSize, y + quarterBlockSize, y + halfBlockSize};

    polygonFill(backx, backy, 4);

    color(0.78, 0.99, 0.93);

    line( x - halfBlockSize, y - quarterBlockSize, x, y);
    line( x + halfBlockSize, y - quarterBlockSize, x, y);
    line( x, y + halfBlockSize, x, y);

    color(2);

    text(x, y, testeX);
    text(x, y + 10, testeY);
}

float xMatrixToScreen(int x, int y)
{
    return (x * 0.5 * blockSize) + (y * -0.5 * blockSize);
}

float yMatrixToScreen(int x, int y)
{
    return (x * 0.25 * blockSize) + (y * 0.25 * blockSize);
}

float xScreenToMatrix(int x, int y)
{
    return (x * 1.0 / blockSize + y * 2.0 / blockSize);
}

float yScreenToMatrix(int x, int y)
{
    return (x * -1.0 / blockSize + y * 2.0 / blockSize);
}

double counter = -360;

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis
//globais que podem ser setadas pelo metodo keyboard()
void render()
{
    float dx =  xScreenToMatrix(mx, my);
    float dy =  yScreenToMatrix(mx, my);

    for (int x = 0; x <= 10; x++){

        counter += 0.001;

        for (int y = -5; y <= 5; y++){
            int xDiff = std::abs(int(x-dx));
            int yDiff = std::abs(int(y-dy));

            counter += 0.00005;

            if (counter > 360){
                counter = -360;
            }

            //if (xDiff == 0 && yDiff == 0){
            //    offSet = quarterBlockSize;
            //}

            double offSet = sin(counter) * 50;

            renderIsometricCube(xMatrixToScreen(x, y), yMatrixToScreen(x, y) - offSet, x, y);
        }
    }

    color(0, 0, 0, 0.1);
    circleFill( mx, my, 10, 10);

    printf("\nmouse X:%d DX:%f Y:%d DY:%f", mx, dx, my, dy);
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

   mx = x; //guarda as coordenadas do mouse para exibir dentro da render()
   my = y;
}

//funcao chamada toda vez que uma tecla for pressionada
void keyboard(int key)
{
   printf("\nClicou Tecla: %d" , key);
}


//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   printf("\nLiberou Tecla: %d" , key);
}


int main(void)
{
    int screenWidth = 500, screenHeight = 500; //largura e altura inicial da tela. Alteram com o redimensionamento de tela.
    init(&screenWidth, &screenHeight, "Canvas 2D");
    run();
}
