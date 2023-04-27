/*********************************************************************
// Canvas para desenho, criada sobre a API OpenGL. Nao eh necessario conhecimentos de OpenGL para usar.
//  Autor: Cesar Tadeu Pozzer
//         05/2020
//
//  Pode ser utilizada para fazer desenhos, animacoes, e jogos simples.
//  Tem tratamento de mouse e teclado
//  Estude o OpenGL antes de tentar compreender o arquivo gl_canvas.cpp
//
//  Versao 2.0
//
//  Instruções:
//	  Para alterar a animacao, digite numeros entre 1 e 3
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "gl_canvas2d.h"
#include "Slider.h"
#include "MouseHandler.h"

using namespace std;

int screenWidth = 500, screenHeight = 500; //largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

float time = 0;
vector<float> wave;
float x, y, oldX, oldY, n, radius;
MouseHandler* mouseHandler = NULL;
Slider* slider = NULL;

void RenderFourier(float xOrigin, float yOrigin)
{
    CV::translate(xOrigin, yOrigin);

    x = 0;
    y = 0;

    for (int i = 0; i < slider->GetValue() * 10; i++)
    {
        oldX = x;
        oldY = y;

        n = i * 2 + 1;
        radius = 75.0 * (4.0 / (n * PI));

        x += radius * cos(n * time);
        y += radius * sin(n * time);

        CV::color(0.5, 0.5, 0.5);
        CV::circle(oldX, oldY, radius, 64);
        CV::circleFill(x, y, 8, 16);

        CV::color(1, 1, 1);
        CV::line(oldX, oldY, x, y);
    }
    wave.insert(wave.begin(), y);

    CV::translate(xOrigin + 150, yOrigin);
    CV::line(x - 150, y, 0, wave[0]);

    for (int i = 0; i < wave.size(); i++)
    {
        CV::point(i * 0.1, wave[i]);
    }

    time += 0.005;

    if (wave.size() > 5000)
    {
        wave.pop_back();
    }
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa função com poucas linhas de codigo.
void render()
{
    CV::clear(0, 0, 0);
    slider->Render();
    RenderFourier(screenWidth/3, screenHeight >> 1);
    CV::translate(screenWidth >> 1, screenHeight >> 1);
    CV::color(3);
    CV::superquadrica(0, 0, 100, 100, slider->GetValue() * 10);
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   printf("\nTecla: %d" , key);
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   printf("\nLiberou: %d" , key);
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    mouseHandler->Update(button, state, wheel, direction, x, y);
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

    if (mouseHandler->GetState() == 0)
    {
        slider->OnMouseClick(mouseHandler->GetX(), mouseHandler->GetY());
    }
    else if (mouseHandler->IsDragging())
    {
        slider->OnMouseDrag(mouseHandler->GetX());
    }
    else if (mouseHandler->GetState() == 1)
    {
        slider->OnMouseRelease();
    }

   //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);
}

int main(void)
{
   CV::init(&screenWidth, &screenHeight, "Titulo da Janela: Canvas 2D - Pressione 1, 2, 3");

   slider = new Slider(15, 15, 100, 15, 0.01, 1, 0.5);
   mouseHandler = new MouseHandler();

   CV::run();
}
