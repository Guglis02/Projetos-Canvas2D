/*
Trabalho 1 – Editor de Figuras Vetoriais
CG - 2023/1
Gustavo Machado de Freitas

O que foi implementado:
● Inserir/excluir figura.
● Cor da figura.
● Preenchimento da figura (Bônus).
● Editar tamanho da figura.
● Editar orientação da figura (giros de 90 graus).
● Enviar para frente/traz.
● Salvar em arquivo e carregar de arquivo.

Extras:
● Sinalizar qual figura está selecionada.
● Rotacionar figura em qualquer ângulo.
● Permitir inserir polígonos quaisquer.
● Alterar cor em espaço HSV.
● Aplicar criptografia no arquivo.
*/
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <stdio.h>

#include "gl_canvas2d.h"
#include "Bars/ToolBar.h"
#include "Bars/ColorBar.h"
#include "DrawingCanvas.h"

using namespace std;

// Largura e altura inicial da tela. Alteram com o redimensionamento de tela.
int screenWidth = 1000, screenHeight = 600;

const int ToolbarHeight = 130;

DrawingCanvas* drawingCanvas = NULL;
ToolBar* toolBar = NULL;
ColorBar* colorBar = NULL;
MouseHandler* mouseHandler = NULL;

// Funcao chamada todo frame
void render()
{
    drawingCanvas->Update(mouseHandler, toolBar->GetCurrentFunction());
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
        if (toolBar->IsMouseInside(mouseHandler->GetX(), mouseHandler->GetY()))
        {
            toolBar->CheckButtonCollision(mouseHandler->GetX(), mouseHandler->GetY());
            drawingCanvas->ResetNewDrawing();
            return;
        }
        if (colorBar->IsMouseInside(mouseHandler->GetX(), mouseHandler->GetY()))
        {
            colorBar->OnMouseClick(mouseHandler->GetX(), mouseHandler->GetY());
            drawingCanvas->ResetNewDrawing();
            return;
        }
        drawingCanvas->OnMouseClick(mouseHandler);
        return;
    }
    // Arrastar o mouse
    if (mouseHandler->IsDragging())
    {
        if (toolBar->IsMouseInside(mouseHandler->GetX(), mouseHandler->GetY()))
        {
            return;
        }
        if (colorBar->IsMouseInside(mouseHandler->GetX(), mouseHandler->GetY()))
        {
            colorBar->OnMouseDrag(mouseHandler->GetX(),
                                  mouseHandler->GetY(),
                                  bind(&DrawingCanvas::UpdateSelectedColor, drawingCanvas, placeholders::_1));
            return;
        }
        drawingCanvas->OnMouseDrag(mouseHandler);
    }
    // Soltar o mouse
    if (mouseHandler->GetState() == 1)
    {
        if (colorBar->IsMouseInside(mouseHandler->GetX(), mouseHandler->GetY()))
        {
            colorBar->OnMouseRelease(mouseHandler->GetX(), mouseHandler->GetY());
            return;
        }
        // Se eu soltar no canvas de desenho
        if (mouseHandler->GetY() > ToolbarHeight)
        {
            drawingCanvas->OnMouseRelease();
        }
        else
        {
            drawingCanvas->ResetNewDrawing();
        }
    }
}

// Funcao chamada toda vez que uma tecla for pressionada
void keyboard(int key)
{
    //printf("\nClicou Tecla: %d", key);
    drawingCanvas->OnKeyboardDown(key);
}

// Funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
    //printf("\nLiberou Tecla: %d" , key);
    drawingCanvas->OnKeyboardUp(key);
}

// Inicia botões da interface
void StartButtons()
{
    toolBar->CreateButton(Rect, nullptr, "Retangulo");
    toolBar->CreateButton(Circle, nullptr, "Circulo");
    toolBar->CreateButton(Triangle, nullptr, "Triangulo");
    toolBar->CreateButton(Poly, nullptr, "Poligono");
    toolBar->CreateButton(Fill, bind(&DrawingCanvas::FillDrawing, drawingCanvas), "Preencher");
    toolBar->CreateButton(BringTop, bind(&DrawingCanvas::BringDrawingTop, drawingCanvas), "Subir");
    toolBar->CreateButton(SendBack, bind(&DrawingCanvas::SendDrawingBack, drawingCanvas), "Descer");
    toolBar->CreateButton(Save, bind(&DrawingCanvas::SaveFile, drawingCanvas), "Salvar");
    toolBar->CreateButton(Delete, bind(&DrawingCanvas::DeleteDrawing, drawingCanvas), "Deletar");
    toolBar->CreateButton(Clear, bind(&DrawingCanvas::ClearCanvas, drawingCanvas), "Limpar");
}

int main(void)
{
    init(&screenWidth, &screenHeight, "Trabalho 1 - Gustavo Machado de Freitas");

    mouseHandler = new MouseHandler();
    toolBar = new ToolBar(ToolbarHeight, screenWidth/2);
    colorBar = new ColorBar(ToolbarHeight, screenWidth/2);
    drawingCanvas = new DrawingCanvas(bind(&ToolBar::DeSelectButton, toolBar),
                                      bind(&ColorBar::SetRGB, colorBar, placeholders::_1));

    StartButtons();

    run();
}
