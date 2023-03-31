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

int height = 130;

vector<Drawing*> drawings;

int tempX = 0;
int tempY = 0;

ToolBar* toolBar = NULL;
ToolBar* colorBar = NULL;
MouseHandler* mouseHandler = NULL;

float selectedColor[] = {0,0,0};

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
        color(selectedColor[0],selectedColor[1],selectedColor[2]);
        rect(tempX, tempY, mouseHandler->x, mouseHandler->y);
    }

    if (mouseHandler->state == 1)
    {
        newDrawing = new RectangleDrawing(tempX, tempY, mouseHandler->x, mouseHandler->y);
        newDrawing->SetColor(selectedColor[0],selectedColor[1],selectedColor[2]);
        drawings.push_back(newDrawing);
        toolBar->DeSelectButton();
    }
}

int temporaryCircleRadius = 0;
void CircleFunction()
{
    if (mouseHandler->isHolding)
    {
        color(selectedColor[0],selectedColor[1],selectedColor[2]);
        temporaryCircleRadius = DistanceBetweenTwoPoints(tempX, tempY, mouseHandler->x, mouseHandler->y);
        circle(tempX, tempY, temporaryCircleRadius, 32);
    }

    if (mouseHandler->state == 1)
    {
        newDrawing = new CircleDrawing(tempX, tempY, temporaryCircleRadius, 32);
        newDrawing->SetColor(selectedColor[0],selectedColor[1],selectedColor[2]);
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
        color(selectedColor[0],selectedColor[1],selectedColor[2]);
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
        newDrawing->SetColor(selectedColor[0],selectedColor[1],selectedColor[2]);
        printf("\n %f %f %f %d %d %d", selectedColor[0],selectedColor[1],selectedColor[2], selectedColor[0],selectedColor[1],selectedColor[2]);
        drawings.push_back(newDrawing);
        toolBar->DeSelectButton();
    }
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis
//globais que podem ser setadas pelo metodo keyboard()
void render()
{
    DrawingsCanvasHandler();
    if (mouseHandler->IsPointerUnder(height))
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
    toolBar->Update(0, 0, height, screenWidth/2);
    colorBar->Update(screenWidth/2, 0, height, screenWidth/2);
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

    mouseHandler->Update(button, state, wheel, direction, x, y);

    if (mouseHandler->IsPointerUnder(height))
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
        if (mouseHandler->state == 0 && toolBar->CheckButtonCollision(mouseHandler->x, mouseHandler->y))
        {
            switch(toolBar->GetCurrentFunction())
            {
                case Clear:
                    drawings.clear();
                    toolBar->DeSelectButton();
                    break;
                default:
                    break;
            }
        } else if (mouseHandler->state == 0 && colorBar->CheckButtonCollision(mouseHandler->x, mouseHandler->y))
        {
            selectedColor[0] = colorBar->selectedButton->r;
            selectedColor[1] = colorBar->selectedButton->g;
            selectedColor[2] = colorBar->selectedButton->b;
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

int defaultButtonWidth = 80;
int defaultButtonHeight = 50;
float defaultButtonColor[] = {0, 0.5, 1};
void StartButtons()
{
    toolBar->CreateButton(defaultButtonHeight, defaultButtonWidth, Rect, "Retangulo", defaultButtonColor);
    toolBar->CreateButton(defaultButtonHeight, defaultButtonWidth, Circle, "Circulo", defaultButtonColor);
    toolBar->CreateButton(defaultButtonHeight, defaultButtonWidth, Triangle, "Triangulo", defaultButtonColor);
    toolBar->CreateButton(defaultButtonHeight, defaultButtonWidth, Poly, "Poligono", defaultButtonColor);
    toolBar->CreateButton(defaultButtonHeight, defaultButtonWidth, Fill, "Preencher", defaultButtonColor);
    toolBar->CreateButton(defaultButtonHeight, defaultButtonWidth, BringTop, "Subir", defaultButtonColor);
    toolBar->CreateButton(defaultButtonHeight, defaultButtonWidth, SendBack, "Descer", defaultButtonColor);
    toolBar->CreateButton(defaultButtonHeight, defaultButtonWidth, Save, "Salvar", defaultButtonColor);
    toolBar->CreateButton(defaultButtonHeight, defaultButtonWidth, Clear, "Limpar", defaultButtonColor);

    for(int i = 0; i < 16; i++)
    {
        float inc = i / 15.0f;
        float r = 0.5f + 0.5f * cos(inc * 2 * PI);
        float g = 0.5f + 0.5f * cos((inc + 1/3.0f) * 2 * PI);
        float b = 0.5f + 0.5f * cos((inc + 2/3.0f) * 2 * PI);
        float rgb[] = {r, g, b};
        colorBar->CreateButton(50, 50, Color, "", rgb);
    }
}

int main(void)
{
    init(&screenWidth, &screenHeight, "Trabalho 1 - Gustavo Machado de Freitas");

    mouseHandler = new MouseHandler();
    toolBar = new ToolBar(height, screenWidth/2);
    colorBar = new ToolBar(height, screenWidth/2);

    StartButtons();

    run();
}
