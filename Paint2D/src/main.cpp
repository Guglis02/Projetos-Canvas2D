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
#include "Button.h"
#include "FunctionType.h"

using namespace std;

int screenWidth = 600, screenHeight = 600; //largura e altura inicial da tela. Alteram com o redimensionamento de tela.

int mx, my; //coordenadas do mouse
int mstate; // estado do mouse
bool isHolding = false;

vector<tuple<int, int>> pointsToDraw;

vector<Button*> buttons;
Button* selectedButton = NULL;

int toolBarHeight = 100;

void ToolBarHandler()
{
    color(1);
    rectFill(0, 0, screenWidth, toolBarHeight);

    for (int i = 0; i < buttons.size(); i++)
    {
        buttons[i]->Update(screenWidth/5 * (i + 1), 50);
        if (buttons[i]->GetSelectedState())
        {
            selectedButton = buttons[i];
        }
    }
}

void DrawingsCanvasHandler()
{
    for (tuple<int, int> p : pointsToDraw)
    {
        color(3);
        point(get<0>(p),get<1>(p));
    }
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis
//globais que podem ser setadas pelo metodo keyboard()
void render()
{
    DrawingsCanvasHandler();
    ToolBarHandler();
}

void PencilTool()
{
    if (isHolding && my >= toolBarHeight)
    {
        pointsToDraw.push_back(make_tuple(mx, my));
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
        if (my < toolBarHeight)
        {
            for (Button* b : buttons)
            {
                b->CheckMouseClick(mx, my);
            }
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

    if(!selectedButton)
    {
        return;
    }

    switch (selectedButton->GetFunction())
    {
        case Pencil:
            PencilTool();
            break;
        case Clear:
            pointsToDraw.clear();
            selectedButton->SetSelectedState(false);
            selectedButton = NULL;
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

   Button* pencilButton = new Button(80, 80, Pencil, "Lapis");
   buttons.push_back(pencilButton);

   Button* clearButton = new Button(80,80, Clear, "Limpar");
   buttons.push_back(clearButton);

   run();
}
