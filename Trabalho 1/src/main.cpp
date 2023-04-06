#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <utility>
#include <algorithm>

#include "gl_canvas2d.h"
#include "ToolBar.h"
#include "FunctionType.h"
#include "Drawings/Drawing.h"
#include "Drawings/RectangleDrawing.h"
#include "Drawings/CircleDrawing.h"
#include "Drawings/TriangleDrawing.h"
#include "Drawings/PolygonDrawing.h"
#include "MouseHandler.h"
#include "FileHandler.h"
#include "PointsUtils.h"

using namespace std;

//Largura e altura inicial da tela. Alteram com o redimensionamento de tela.
int screenWidth = 1000, screenHeight = 600;

const int ToolbarHeight = 130;

vector<Drawing*> drawings;

bool drawingMode = false;

int tempX = 0;
int tempY = 0;

vector<float> tempXs;
vector<float> tempYs;

ToolBar* toolBar = NULL;
ToolBar* colorBar = NULL;
MouseHandler* mouseHandler = NULL;

Drawing* newDrawing;
Drawing* selectedDrawing;

float selectedColor[] = {0,0,0};

void DrawingsCanvasHandler()
{
    for (Drawing* d : drawings)
    {
        d->Render();
    }

    if (selectedDrawing)
    {
        selectedDrawing->RenderSelectionIndicators();
    }
}

void CheckDrawingSelection()
{
    for (Drawing* d : drawings)
    {
        if (d->CheckMouseClick(mouseHandler->x, mouseHandler->y))
        {
            selectedDrawing = d;
            return;
        }
    }
    selectedDrawing = NULL;
}

void StartDrawing()
{
    newDrawing = NULL;
    selectedDrawing = NULL;

    tempX = mouseHandler->x;
    tempY = mouseHandler->y;

    drawingMode = true;
}



const int circleIndicatorRadius = 5;
void PolygonFunction()
{
    if (tempXs.size() != 0)
    {
        polygon(tempXs.data(), tempYs.data(), tempXs.size());

        for (int i = 0; i < tempXs.size(); i++)
        {
            circle(tempXs[i], tempYs[i], circleIndicatorRadius, 10);
        }
    }
}

void AddDrawing()
{
    newDrawing->SetColor(selectedColor[0],selectedColor[1],selectedColor[2]);
    selectedDrawing = newDrawing;
    drawings.push_back(newDrawing);
    toolBar->DeSelectButton();
    drawingMode = false;
    tempXs.clear();
    tempYs.clear();
}

int moveInc[2] = {0, 0};

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis
//globais que podem ser setadas pelo metodo keyboard()
void render()
{
    DrawingsCanvasHandler();

    color(selectedColor[0],selectedColor[1],selectedColor[2]);
    if (drawingMode && mouseHandler->isHolding)
    {
        newDrawing->RenderPrototype(tempX, tempY, mouseHandler->x, mouseHandler->y);
    }
    if (toolBar->GetCurrentFunction() == Poly)
    {
        PolygonFunction();
    }
    if (selectedDrawing && mouseHandler->isDragging)
    {
        selectedDrawing->Move(mouseHandler->x - selectedDrawing->GetCenterX(),
                              mouseHandler->y - selectedDrawing->GetCenterY());
    }
    if (selectedDrawing && selectedDrawing->isMoving)
    {
        selectedDrawing->Move(moveInc[0], moveInc[1]);
    }

    toolBar->Update(0, 0, ToolbarHeight, screenWidth/2);
    colorBar->Update(screenWidth/2, 0, ToolbarHeight, screenWidth/2);
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

    mouseHandler->Update(button, state, wheel, direction, x, y);

    // Clicar com o mouse
    if (mouseHandler->state == 0)
    {
        // Se eu cliquei na barra de botões
        if (mouseHandler->IsPointerOver(ToolbarHeight))
        {
            // Se eu cliquei em um botão de função
            if (toolBar->CheckButtonCollision(mouseHandler->x, mouseHandler->y))
            {
                switch(toolBar->GetCurrentFunction())
                {
                    case Fill:
                        if (selectedDrawing)
                        {
                            selectedDrawing->SwitchFillable();
                        }
                        toolBar->DeSelectButton();
                        break;
                    case BringTop:
                        if (selectedDrawing && selectedDrawing != drawings.back())
                        {
                            iter_swap(find(drawings.begin(), drawings.end(), selectedDrawing),
                                      find(drawings.begin(), drawings.end(), selectedDrawing) + 1);
                        }
                        toolBar->DeSelectButton();
                        break;
                    case SendBack:
                        if (selectedDrawing && selectedDrawing != drawings.front())
                        {
                            iter_swap(find(drawings.begin(), drawings.end(), selectedDrawing),
                                      find(drawings.begin(), drawings.end(), selectedDrawing) - 1);
                        }
                        toolBar->DeSelectButton();
                        break;
                    case Delete:
                        if (selectedDrawing)
                        {
                            drawings.erase(find(drawings.begin(), drawings.end(), selectedDrawing));
                            delete selectedDrawing;
                            selectedDrawing = NULL;
                        }
                        toolBar->DeSelectButton();
                        break;
                    case Save:
                        SaveInFile(drawings);
                        toolBar->DeSelectButton();
                        break;
                    case Clear:
                        drawings.clear();
                        toolBar->DeSelectButton();
                        selectedDrawing = NULL;
                        break;
                    default:
                        break;
                }
            }
            // Se eu cliquei em um botão de cor
            else if (colorBar->CheckButtonCollision(mouseHandler->x, mouseHandler->y))
            {
                selectedColor[0] = colorBar->selectedButton->r;
                selectedColor[1] = colorBar->selectedButton->g;
                selectedColor[2] = colorBar->selectedButton->b;
                if (selectedDrawing)
                {
                    selectedDrawing->SetColor(selectedColor[0],selectedColor[1],selectedColor[2]);
                }
                colorBar->DeSelectButton();
            }

            tempXs.clear();
            tempYs.clear();
        }
        // Se eu cliquei no canvas de desenho
        else
        {
            switch(toolBar->GetCurrentFunction())
            {
                case Rect:
                    StartDrawing();
                    newDrawing = new RectangleDrawing(tempX,
                                  tempY,
                                  mouseHandler->x,
                                  mouseHandler->y);
                    newDrawing->SetColor(selectedColor[0],selectedColor[1],selectedColor[2]);
                    break;
                case Circle:
                    StartDrawing();
                    newDrawing = new CircleDrawing(tempX,
                                                   tempY,
                                                   1,
                                                   32);
                    newDrawing->SetColor(selectedColor[0],selectedColor[1],selectedColor[2]);
                    break;
                case Triangle:
                    StartDrawing();
                    newDrawing = new TriangleDrawing(tempX,
                                                    tempY,
                                                    (mouseHandler->x - tempX),
                                                    (mouseHandler->y - tempY));
                    break;
                case Poly:
                    selectedDrawing = NULL;
                    if (pnpoly(tempXs.size(), tempXs.data(), tempYs.data(), mouseHandler->x, mouseHandler->y))
                    {
                        int* xs = new int[tempXs.size()];
                        int* ys = new int[tempXs.size()];

                        for (int i = 0; i < tempXs.size(); i++) {
                            xs[i] = static_cast<int>(tempXs[i]);
                            ys[i] = static_cast<int>(tempYs[i]);
                        }

                        newDrawing = new PolygonDrawing(xs, ys, tempXs.size());
                        AddDrawing();
                    } else
                    {
                        tempXs.push_back(static_cast<float>(mouseHandler->x));
                        tempYs.push_back(static_cast<float>(mouseHandler->y));
                    }
                    break;
                case None:
                    // Checa a colisão com as figuras na tela
                    CheckDrawingSelection();
                    break;
                default:
                    break;
            }
        }
    }

    // Soltar o mouse
    if (mouseHandler->state == 1)
    {
        // Se eu soltar na barra de botões
        if (mouseHandler->IsPointerOver(ToolbarHeight))
        {
            if (drawingMode)
            {
                drawingMode = false;
            }
        }
        // Se eu soltar no canvas de desenho
        else
        {
            if (drawingMode)
            {
                AddDrawing();
            }
        }
    }
}


//funcao chamada toda vez que uma tecla for pressionada
void keyboard(int key)
{
   printf("\nClicou Tecla: %d" , key);

   if (key == 26 && !drawings.empty())
   {
        printf("\nClicou Tecla: %d" , key);
        drawings.pop_back();
   }

   if (selectedDrawing)
   {
       switch(key)
       {
            case 200:
                moveInc[0] = -10;
                moveInc[1] = 0;
                selectedDrawing->isMoving = true;
                break;
            case 201:
                moveInc[0] = 0;
                moveInc[1] = -10;
                selectedDrawing->isMoving = true;
                break;
            case 202:
                moveInc[0] = 10;
                moveInc[1] = 0;
                selectedDrawing->isMoving = true;
                break;
            case 203:
                moveInc[0] = 0;
                moveInc[1] = 10;
                selectedDrawing->isMoving = true;
                break;
            default:
                break;
       }
   }
}


//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   //printf("\nLiberou Tecla: %d" , key);

   if (selectedDrawing)
   {
        moveInc[0] = 0;
        moveInc[1] = 0;
        selectedDrawing->isMoving = false;
   }
}

void StartButtons()
{
    toolBar->CreateButton(Rect, "Retangulo");
    toolBar->CreateButton(Circle, "Circulo");
    toolBar->CreateButton(Triangle, "Triangulo");
    toolBar->CreateButton(Poly, "Poligono");
    toolBar->CreateButton(Fill, "Preencher");
    toolBar->CreateButton(BringTop, "Subir");
    toolBar->CreateButton(SendBack, "Descer");
    toolBar->CreateButton(Save, "Salvar");
    toolBar->CreateButton(Delete, "Deletar");
    toolBar->CreateButton(Clear, "Limpar");

    for(int i = 0; i < 16; i++)
    {
        float inc = i / 16.0f;
        float r = 0.5f + 0.5f * cos(inc * PI_2);
        float g = 0.5f + 0.5f * cos((inc + 1/3.0f) * PI_2);
        float b = 0.5f + 0.5f * cos((inc + 2/3.0f) * PI_2);
        float rgb[] = {r, g, b};
        colorBar->CreateButton(50, 50, Color, "", rgb);
    }
}

int main(void)
{
    init(&screenWidth, &screenHeight, "Trabalho 1 - Gustavo Machado de Freitas");

    mouseHandler = new MouseHandler();
    toolBar = new ToolBar(ToolbarHeight, screenWidth/2);
    colorBar = new ToolBar(ToolbarHeight, screenWidth/2);

    StartButtons();

    LoadFromFile(drawings);

    run();
}
