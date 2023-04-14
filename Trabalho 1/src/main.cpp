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
● (+0,5) Sinalizar qual figura está selecionada.
● (+1,0) Rotacionar figura em qualquer ângulo.
● (+1,0) Permitir inserir polígonos quaisquer.
*/
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <stdio.h>

#include "gl_canvas2d.h"
#include "ToolBar.h"
#include "DrawingCanvas.h"

using namespace std;

//Largura e altura inicial da tela. Alteram com o redimensionamento de tela.
int screenWidth = 1000, screenHeight = 600;

const int ToolbarHeight = 130;

DrawingCanvas* drawingCanvas = NULL;
ToolBar* toolBar = NULL;
ToolBar* colorBar = NULL;
MouseHandler* mouseHandler = NULL;

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis
//globais que podem ser setadas pelo metodo keyboard()
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
        // Se eu cliquei na barra de botões
        if (mouseHandler->IsPointerOver(ToolbarHeight))
        {
            toolBar->CheckButtonCollision(mouseHandler->GetX(), mouseHandler->GetY());

            if (colorBar->CheckButtonCollision(mouseHandler->GetX(), mouseHandler->GetY()))
            {
                drawingCanvas->UpdateSelectedColor(colorBar->GetSelectedButton()->r,
                                                   colorBar->GetSelectedButton()->g,
                                                   colorBar->GetSelectedButton()->b);
                colorBar->DeSelectButton();
            }

            drawingCanvas->ResetNewDrawing();
            return;
        }
        drawingCanvas->MouseClick(mouseHandler);
        return;
    }

    if (mouseHandler->IsDragging())
    {
        if (mouseHandler->IsPointerUnder(ToolbarHeight))
        {
            drawingCanvas->MouseDrag(mouseHandler);
        }
    }

    // Soltar o mouse
    if (mouseHandler->GetState() == 1)
    {
        // Se eu soltar no canvas de desenho
        if (mouseHandler->IsPointerUnder(ToolbarHeight))
        {
            drawingCanvas->MouseRelease();
        }
        else
        {
            drawingCanvas->ResetNewDrawing();
        }
    }
}

//funcao chamada toda vez que uma tecla for pressionada
void keyboard(int key)
{
    printf("\nClicou Tecla: %d" , key);
    drawingCanvas->Keyboard(key);
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
    //printf("\nLiberou Tecla: %d" , key);
    drawingCanvas->KeyboardUp(key);
}

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
    drawingCanvas = new DrawingCanvas(bind(&ToolBar::DeSelectButton, toolBar));

    StartButtons();

    run();
}
