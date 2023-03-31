#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "gl_canvas2d.h"
#include "ToolBar.h"
#include "FunctionType.h"
#include "Drawings/Drawing.h"
#include "Drawings/RectangleDrawing.h"
#include "Drawings/CircleDrawing.h"
#include "MouseHandler.h"
#include "PointsUtils.h"

using namespace std;

int screenWidth = 800, screenHeight = 600; //largura e altura inicial da tela. Alteram com o redimensionamento de tela.

int toolBarHeight = 100;

vector<Drawing*> drawings;

int tempX = 0;
int tempY = 0;

ToolBar* toolBar = NULL;
MouseHandler* mouseHandler = NULL;

void DrawingsCanvasHandler()
{
    for (Drawing* d : drawings)
    {
        d->Render();
    }
}

Drawing* newDrawing;

void StartDrawing()
{
    newDrawing = NULL;

    if (mouseHandler->state == 0)
    {
        tempX = mouseHandler->x;
        tempY = mouseHandler->y;
    }
}

void RectFunction()
{
    if (mouseHandler->isHolding)
    {
        color(2);
        rect(tempX, tempY, mouseHandler->x, mouseHandler->y);
    }

    if (mouseHandler->state == 1)
    {
        newDrawing = new RectangleDrawing(tempX, tempY, mouseHandler->x, mouseHandler->y);
        newDrawing->SetColor(0,0,150);
        drawings.push_back(newDrawing);
        toolBar->DeSelectButton();
    }
}

int temporaryCircleRadius = 0;
void CircleFunction()
{
    if (mouseHandler->isHolding)
    {
        color(2);
        temporaryCircleRadius = DistanceBetweenTwoPoints(tempX, tempY, mouseHandler->x, mouseHandler->y);
        circle(tempX, tempY, temporaryCircleRadius, 32);
    }

    if (mouseHandler->state == 1)
    {
        newDrawing = new CircleDrawing(tempX, tempY, temporaryCircleRadius, 32);
        newDrawing->SetColor(0,0,150);
        drawings.push_back(newDrawing);
        toolBar->DeSelectButton();
    }
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis
//globais que podem ser setadas pelo metodo keyboard()
void render()
{
    DrawingsCanvasHandler();
    if (mouseHandler->IsPointerUnder(toolBarHeight))
    {
        switch(toolBar->GetCurrentFunction())
        {
            case Rect:
                RectFunction();
                break;
            case Circle:
                CircleFunction();
                break;
            default:
                break;
        }
    }
    toolBar->Update(toolBarHeight, screenWidth);
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

    mouseHandler->Update(button, state, wheel, direction, x, y);

    if (mouseHandler->IsPointerUnder(toolBarHeight))
    {
        switch(toolBar->GetCurrentFunction())
        {
            case Rect:
            case Circle:
                StartDrawing();
                break;
            default:
                break;
        }
    }
    else
    {
        if (mouseHandler->state == 0)
        {
            toolBar->CheckButtonCollision(mouseHandler->x, mouseHandler->y);
        }
        switch(toolBar->GetCurrentFunction())
        {
            case Clear:
                drawings.clear();
                toolBar->DeSelectButton();
                break;
            default:
                break;
        }
    }
}

//funcao chamada toda vez que uma tecla for pressionada
void keyboard(int key)
{
   //printf("\nClicou Tecla: %d" , key);

   if (key == 26 && !drawings.empty())
   {
        printf("\nClicou Tecla: %d" , key);
        drawings.pop_back();
   }
}


//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   //printf("\nLiberou Tecla: %d" , key);
}


int main(void)
{
   init(&screenWidth, &screenHeight, "Trabalho 1 - Gustavo Machado de Freitas");

   mouseHandler = new MouseHandler();
   toolBar = new ToolBar(toolBarHeight, screenWidth);

   run();
}
