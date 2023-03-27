#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <vector>
#include <tuple>
#include <list>
#include "gl_canvas2d.h"
#include "gl_canvas2d.h"
#include "ToolBar.h"
#include "FunctionType.h"
#include "Drawing.h"

using namespace std;

int screenWidth = 600, screenHeight = 600; //largura e altura inicial da tela. Alteram com o redimensionamento de tela.

int mx, my; //coordenadas do mouse
int mstate; // estado do mouse
bool isHolding = false;

int toolBarHeight = 100;

vector<Drawing*> drawings;

int rectX = 0;
int rectY = 0;

ToolBar* toolBar = NULL;

void DrawingsCanvasHandler()
{
    for (Drawing* d : drawings)
    {
        d->Update();
    }

    if (isHolding && toolBar->selectedButton != NULL && toolBar->selectedButton->GetFunction() == Rect)
    {
        color(2);
        rect(rectX, rectY, mx, my);
    }
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis
//globais que podem ser setadas pelo metodo keyboard()
void render()
{
    DrawingsCanvasHandler();
    toolBar->Update(screenHeight, screenWidth);
}

void PencilTool()
{
    if (isHolding && my >= toolBarHeight)
    {
        Drawing* drawing = new Drawing(mx, my, Pencil);
        drawings.push_back(drawing);
    }
}

void RectTool()
{
    if (mstate == 0)
    {
        rectX = mx;
        rectY = my;
    }

    if(mstate == 1)
    {
        Drawing* drawing = new Drawing(rectX, rectY, mx, my, Rect);
        drawings.push_back(drawing);
    }
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

    mx = x; //guarda as coordenadas do mouse para exibir dentro da render()
    my = y;

    mstate = state;

    if (mstate == 0)
    {
       // Se o clique foi na barra de ferramentas
        if (my < toolBarHeight)
        {
            toolBar->CheckButtonCollision(mx, my);
        }
        else
        {
            isHolding = true;
        }
    }
    if (mstate == 1)
    {
        isHolding = false;
    }

    if(!toolBar->selectedButton)
    {
        return;
    }

    switch (toolBar->selectedButton->GetFunction())
    {
        case Pencil:
            PencilTool();
            break;
        case Rect:
            RectTool();
            break;
        case Clear:
            drawings.clear();
            toolBar->selectedButton->SetSelectedState(false);
            toolBar->selectedButton = NULL;
            break;
        default:
            break;
    }
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
   init(&screenWidth, &screenHeight, "Paint 2D");

   toolBar = new ToolBar(toolBarHeight);
   run();
}
