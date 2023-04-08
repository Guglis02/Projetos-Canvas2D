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

vector<Vector2> tempPoints;

ToolBar* toolBar = NULL;
ToolBar* colorBar = NULL;
MouseHandler* mouseHandler = NULL;

Drawing* newDrawing;
Drawing* selectedDrawing;

float selectedColor[] = {0,0,0};

// Callbacks
void FillDrawing(void)
{
    if (selectedDrawing)
    {
        selectedDrawing->SwitchFillable();
    }
    toolBar->DeSelectButton();
}

void BringDrawingTop(void)
{
    if (selectedDrawing && selectedDrawing != drawings.back())
    {
        iter_swap(find(drawings.begin(), drawings.end(), selectedDrawing),
                  find(drawings.begin(), drawings.end(), selectedDrawing) + 1);
    }
    toolBar->DeSelectButton();
}

void SendDrawingBack(void)
{
    if (selectedDrawing && selectedDrawing != drawings.front())
    {
        iter_swap(find(drawings.begin(), drawings.end(), selectedDrawing),
                  find(drawings.begin(), drawings.end(), selectedDrawing) - 1);
    }
    toolBar->DeSelectButton();
}

void DeleteDrawing(void)
{
    if (selectedDrawing)
    {
        drawings.erase(find(drawings.begin(), drawings.end(), selectedDrawing));
        delete selectedDrawing;
        selectedDrawing = NULL;
    }
    toolBar->DeSelectButton();
}

void SaveFile(void)
{
    SaveInFile(drawings);
    toolBar->DeSelectButton();
}

void ClearCanvas(void)
{
    drawings.clear();
    toolBar->DeSelectButton();
    selectedDrawing = NULL;
}
//
// Métodos da render
const int circleIndicatorRadius = 5;
void RenderPolygonPrototype()
{
    int numberOfPoints = tempPoints.size();
    if (numberOfPoints != 0)
    {
        polygon(Vector2::GetXs(tempPoints.data(), numberOfPoints),
                Vector2::GetYs(tempPoints.data(), numberOfPoints),
                numberOfPoints);

        for (int i = 0; i < numberOfPoints; i++)
        {
            circle(tempPoints[i].x, tempPoints[i].y, circleIndicatorRadius, 10);
        }
    }
}

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

    color(selectedColor[0],selectedColor[1],selectedColor[2]);
    if (newDrawing && mouseHandler->IsHolding())
    {
        newDrawing->RenderPrototype(mouseHandler->GetClickX(),
                                    mouseHandler->GetClickY(),
                                    mouseHandler->GetX(),
                                    mouseHandler->GetY());
    }

    if (toolBar->GetCurrentFunction() == Poly)
    {
        RenderPolygonPrototype();
    }
}
//

void CheckDrawingSelection()
{
    for (Drawing* d : drawings)
    {
        if (d->CheckMouseClick(mouseHandler->GetX(), mouseHandler->GetY()))
        {
            selectedDrawing = d;
            return;
        }
    }
    selectedDrawing = NULL;
}

void UpdateSelectedColor()
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

void AddDrawing()
{
    newDrawing->SetColor(selectedColor[0],selectedColor[1],selectedColor[2]);
    selectedDrawing = newDrawing;
    drawings.push_back(newDrawing);
    newDrawing = NULL;
    toolBar->DeSelectButton();
    tempPoints.clear();
}

Vector2 moveInc;

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis
//globais que podem ser setadas pelo metodo keyboard()
void render()
{
    DrawingsCanvasHandler();

    // Usado apenas para controlar movimento do desenho com o teclado
    if (selectedDrawing && selectedDrawing->isMoving)
    {
        selectedDrawing->Move(moveInc);
    }

    toolBar->Update(0, 0, ToolbarHeight, screenWidth/2);
    colorBar->Update(screenWidth/2, 0, ToolbarHeight, screenWidth/2);
}

// Funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

    mouseHandler->Update(button, state, wheel, direction, x, y);

    // Clicar com o mouse
    if (mouseHandler->GetState() == 0)
    {
        // Se eu cliquei na barra de botões
        if (mouseHandler->IsPointerOver(ToolbarHeight))
        {
            toolBar->CheckButtonCollision(mouseHandler->GetX(), mouseHandler->GetY());

            if (colorBar->CheckButtonCollision(mouseHandler->GetX(), mouseHandler->GetY()))
            {
                UpdateSelectedColor();
            }

            tempPoints.clear();
            return;
        }
        switch(toolBar->GetCurrentFunction())
        {
            case Rect:
                selectedDrawing = NULL;
                newDrawing = new RectangleDrawing(mouseHandler->GetClickX(),
                                                  mouseHandler->GetClickY(),
                                                  mouseHandler->GetX(),
                                                  mouseHandler->GetY());
                newDrawing->SetColor(selectedColor[0],selectedColor[1],selectedColor[2]);
                break;
            case Circle:
                selectedDrawing = NULL;
                newDrawing = new CircleDrawing(mouseHandler->GetClickX(),
                                               mouseHandler->GetClickY(),
                                               mouseHandler->GetDiffX(),
                                               32);
                newDrawing->SetColor(selectedColor[0],selectedColor[1],selectedColor[2]);
                break;
            case Triangle:
                selectedDrawing = NULL;
                newDrawing = new TriangleDrawing(mouseHandler->GetClickX(),
                                                 mouseHandler->GetClickY(),
                                                 mouseHandler->GetDiffX(),
                                                 mouseHandler->GetDiffY());
                newDrawing->SetColor(selectedColor[0],selectedColor[1],selectedColor[2]);
                break;
            case Poly:
                selectedDrawing = NULL;
                if (pnpoly(tempPoints.size(), tempPoints.data(), Vector2(mouseHandler->GetX(), mouseHandler->GetY())))
                {
                    newDrawing = new PolygonDrawing(Vector2::GetXs(tempPoints.data(), tempPoints.size()),
                                                    Vector2::GetYs(tempPoints.data(), tempPoints.size()),
                                                    tempPoints.size());
                    AddDrawing();
                } else
                {
                    tempPoints.push_back(Vector2(mouseHandler->GetX(), mouseHandler->GetY()));
                }
                break;
            default:
                if (selectedDrawing && selectedDrawing->CheckMouseInteraction(mouseHandler->GetX(), mouseHandler->GetY()))
                {
                    return;
                }
                CheckDrawingSelection();
                break;
        }
        return;
    }

    if (selectedDrawing && mouseHandler->IsDragging())
    {
        selectedDrawing->EditDrawing(mouseHandler->GetDiffX(),
                              mouseHandler->GetDiffY());
    }

    // Soltar o mouse
    if (mouseHandler->GetState() == 1)
    {
        // Se eu soltar no canvas de desenho
        if (mouseHandler->IsPointerUnder(ToolbarHeight))
        {
            if (newDrawing && toolBar->GetCurrentFunction() != Poly)
            {
                AddDrawing();
            }
        }
        else
        {
            newDrawing = NULL;
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
        selectedDrawing = NULL;
   }

   if (selectedDrawing)
   {
       switch(key)
       {
            case 200:
                moveInc.set(-10, 0);
                selectedDrawing->isMoving = true;
                break;
            case 201:
                moveInc.set(0, -10);
                selectedDrawing->isMoving = true;
                break;
            case 202:
                moveInc.set(10, 0);
                selectedDrawing->isMoving = true;
                break;
            case 203:
                moveInc.set(0, 10);
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
        moveInc.set(0, 0);
        selectedDrawing->isMoving = false;
   }
}

void StartButtons()
{
    toolBar->CreateButton(Rect, nullptr, "Retangulo");
    toolBar->CreateButton(Circle, nullptr, "Circulo");
    toolBar->CreateButton(Triangle, nullptr, "Triangulo");
    toolBar->CreateButton(Poly, nullptr, "Poligono");
    toolBar->CreateButton(Fill, FillDrawing, "Preencher");
    toolBar->CreateButton(BringTop, BringDrawingTop, "Subir");
    toolBar->CreateButton(SendBack, SendDrawingBack, "Descer");
    toolBar->CreateButton(Save, SaveFile, "Salvar");
    toolBar->CreateButton(Delete, DeleteDrawing, "Deletar");
    toolBar->CreateButton(Clear, ClearCanvas, "Limpar");

    for(int i = 0; i < 16; i++)
    {
        float inc = i / 16.0f;
        float r = 0.5f + 0.5f * cos(inc * PI_2);
        float g = 0.5f + 0.5f * cos((inc + 1/3.0f) * PI_2);
        float b = 0.5f + 0.5f * cos((inc + 2/3.0f) * PI_2);
        float rgb[] = {r, g, b};
        colorBar->CreateButton(50, 50, Color, nullptr, "", rgb);
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
