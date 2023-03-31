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
#include "Drawings/TriangleDrawing.h"
#include "MouseHandler.h"
#include "PointsUtils.h"

using namespace std;

int screenWidth = 1000, screenHeight = 600; //largura e altura inicial da tela. Alteram com o redimensionamento de tela.

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

float temporaryX[3];
float temporaryY[3];
void TriangleFunction()
{
    if (mouseHandler->isHolding)
    {
        color(2);
        temporaryX[0] = tempX;
        temporaryY[0] = tempY + (tempY - mouseHandler->y);
        temporaryX[1] = tempX + abs(tempX - mouseHandler->x) * 0.5;
        temporaryY[1] = tempY;
        temporaryX[2] = tempX + abs(tempX - mouseHandler->x);
        temporaryY[2] = tempY + (tempY - mouseHandler->y);
        polygon(temporaryX, temporaryY, 3);
    }

    if (mouseHandler->state == 1)
    {
        newDrawing = new TriangleDrawing(tempX, tempY, abs(tempX - mouseHandler->x), (tempY - mouseHandler->y));
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
            case Triangle:
                TriangleFunction();
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
            case Triangle:
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

int defaultButtonColor[3] = {0, 0, 1};
void StartButtons()
{
    toolBar->CreateButton(80, 80, Rect, "Retangulo", defaultButtonColor);
    toolBar->CreateButton(80, 80, Circle, "Circulo", defaultButtonColor);
    toolBar->CreateButton(80, 80, Triangle, "Triangulo", defaultButtonColor);
    toolBar->CreateButton(80, 80, Poly, "Poligono", defaultButtonColor);
    toolBar->CreateButton(80, 80, Fill, "Preencher", defaultButtonColor);
    toolBar->CreateButton(80, 80, BringTop, "Subir", defaultButtonColor);
    toolBar->CreateButton(80, 80, SendBack, "Descer", defaultButtonColor);
    toolBar->CreateButton(80, 80, Save, "Salvar", defaultButtonColor);
    toolBar->CreateButton(80, 80, Clear, "Limpar", defaultButtonColor);
}

int main(void)
{
    init(&screenWidth, &screenHeight, "Trabalho 1 - Gustavo Machado de Freitas");

    mouseHandler = new MouseHandler();
    toolBar = new ToolBar(toolBarHeight, screenWidth);

    StartButtons();

    run();
}
