#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <stdio.h>

#include "gl_canvas2d.h"
#include "MouseHandler.h"
#include "Chart.h"

using namespace std;

// Largura e altura inicial da tela. Alteram com o redimensionamento de tela.
int screenWidth = 1400, screenHeight = 700;

MouseHandler* mouseHandler = NULL;
vector<Chart*> charts;
Chart* inputChart = NULL;
Chart* idctChart = NULL;
Chart* dctChart = NULL;
Chart* diffChart = NULL;

// Funcao chamada todo frame
void render()
{
    for (Chart* chart : charts)
    {
        chart->Update(screenWidth * 0.4, screenHeight * 0.3);
        chart->Render();
    }
}

// Funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

    mouseHandler->Update(button, state, wheel, direction, x, y);

    // Clicar com o mouse
    if (mouseHandler->GetState() == 0)
    {
        return;
    }
    // Arrastar o mouse
    if (mouseHandler->IsDragging())
    {
        return;
    }
    // Soltar o mouse
    if (mouseHandler->GetState() == 1)
    {
        return;
    }
}

// Funcao chamada toda vez que uma tecla for pressionada
void keyboard(int key)
{
    //printf("\nClicou Tecla: %d", key);
}

// Funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
    //printf("\nLiberou Tecla: %d" , key);
}

int main(void)
{
    CV::init(&screenWidth, &screenHeight, "Trabalho 2 - Gustavo Machado de Freitas");

    mouseHandler = new MouseHandler();
    vector<Vector2> points;
    for (float i = 0; i <= 100; i++)
    {
        points.push_back(Vector2(i, i * 2.0));
    }

    inputChart = new Chart(100, 150 + screenHeight * 0.3, screenWidth * 0.4, screenHeight * 0.3, points, "Input");
    idctChart = new Chart(100 + 100 + screenWidth * 0.4, 150 + screenHeight * 0.3, screenWidth * 0.4, screenHeight * 0.3, points, "IDCT");
    dctChart = new Chart(100, 100, screenWidth * 0.4, screenHeight * 0.3, points, "DCT");
    diffChart = new Chart(100 + 100 + screenWidth * 0.4, 100, screenWidth * 0.4, screenHeight * 0.3, points, "Diff");

    charts.push_back(inputChart);
    charts.push_back(idctChart);
    charts.push_back(dctChart);
    charts.push_back(diffChart);

    CV::run();
}
