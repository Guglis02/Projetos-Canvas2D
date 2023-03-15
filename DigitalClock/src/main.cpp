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
#include <chrono>
#include "gl_canvas2d.h"
#include "gl_canvas2d.h"
#include "7SegmentsDisplay.h"


int mx, my; //coordenadas do mouse
int screenWidth = 600, screenHeight = 600; //largura e altura inicial da tela. Alteram com o redimensionamento de tela.
int timeArray[4] = {0, 0, 0, 0};


void getTime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t time_now = std::chrono::system_clock::to_time_t(now);
    std::tm *ptm = std::localtime(&time_now);

    int hour = ptm->tm_hour;
    int minute = ptm->tm_min;

    timeArray[0] = hour / 10;
    timeArray[1] = hour % 10;
    timeArray[2] = minute / 10;
    timeArray[3] = minute % 10;
}

void renderFirstClockFrame(float xOffset, float yOffset)
{
    float width = screenWidth - 2 * xOffset;
    float height = screenHeight - 2 * yOffset;

    // Alarm Button
    color(0.99, 0.69, 0.25);
    rectFillWithRoundedCorners(xOffset + width / 4, yOffset - 15, xOffset + 3 * width / 4, yOffset + 10, 10, 50);
    // Orange Frame
    color(0.96, 0.39, 0.13);
    rectFillWithRoundedCorners(xOffset, yOffset, xOffset + width, yOffset + height, 50, 50);
    // White Body
    color(0.94, 0.96, 0.95);
    rectFillWithRoundedCorners(xOffset + 10, yOffset + 10, xOffset + width - 10, yOffset + height - 10, 50, 50);
    // Screen Frame
    color(0.81, 0.83, 0.82);
    rectFill(xOffset + width / 6 - 5, yOffset + 20, xOffset + 5 * width / 6 + 5, yOffset + height - 20);
    // Screen Shadow
    color(0.46, 0.67, 0.66);
    rectFill(xOffset + width / 6, yOffset + 25, xOffset + 5 * width / 6, yOffset + height - 25);
    // Display
    color(0.51, 0.77, 0.75);
    rectFill(xOffset + width / 6 + 5, yOffset + 30, xOffset + 5 * width / 6, yOffset + height - 25);

    getTime();

    color(0.22, 0.23, 0.24);
    Draw7SegmentsDisplay(190, 260, 6, 2, timeArray[0]);
    Draw7SegmentsDisplay(245, 260, 6, 2, timeArray[1]);
    Draw7SegmentsDisplay(315, 260, 6, 2, timeArray[2]);
    Draw7SegmentsDisplay(370, 260, 6, 2, timeArray[3]);
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis
//globais que podem ser setadas pelo metodo keyboard()
void render()
{
    renderFirstClockFrame(100, 210);
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

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
   init(&screenWidth, &screenHeight, "Canvas 2D");
   run();
}
