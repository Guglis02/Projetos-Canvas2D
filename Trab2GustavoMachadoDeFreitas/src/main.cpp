#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <stdio.h>
#include <cstdlib>
#include <ctime>

#include "gl_canvas2d.h"
#include "MouseHandler.h"
#include "Chart.h"
#include "Transformations.h"
#include "InteractableBar.h"
#include "ChartManager.h"

using namespace std;

// Largura e altura inicial da tela. Alteram com o redimensionamento de tela.
int screenWidth = 1400, screenHeight = 700;
const int barHeight = 120;

MouseHandler* mouseHandler = NULL;
InteractableBar* buttonBar = NULL;
ChartManager* chartManager = NULL;

// Funcao chamada todo frame
void render()
{
    buttonBar->Update(barHeight, screenWidth);
    chartManager->Update(screenWidth, screenHeight);
}

// Funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

    mouseHandler->Update(button, state, wheel, direction, x, y);

    // Clicar com o mouse
    if (mouseHandler->GetState() == 0)
    {
        buttonBar->CheckMouseClick(mouseHandler->GetX(),
                                   mouseHandler->GetY());
        return;
    }
    // Arrastar o mouse
    if (mouseHandler->IsDragging())
    {
        buttonBar->OnMouseDrag(mouseHandler->GetX());
        return;
    }
    // Soltar o mouse
    if (mouseHandler->GetState() == 1)
    {
        buttonBar->OnMouseRelease();
        return;
    }
}

// Funcao chamada toda vez que uma tecla for pressionada
void keyboard(int key) {}
// Funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key) {}

void StartButtons()
{
    buttonBar->CreateSlider("Quantitizacao", 1, 200, 10, bind(&ChartManager::SetQuantitizationCoefficient, chartManager, placeholders::_1));
    buttonBar->CreateSlider("Amostras", 32, 512, 200, bind(&ChartManager::SetNumberOfValues, chartManager, placeholders::_1));
    buttonBar->CreateSlider("Freq", 1, 50, 2, bind(&ChartManager::SetValuesFreq, chartManager, placeholders::_1));
    buttonBar->CreateSlider("Ampli", 0, 127, 100, bind(&ChartManager::SetValuesAmp, chartManager, placeholders::_1));

    buttonBar->CreateButton("Gerar In", "Random", bind(&ChartManager::GenerateRandomInput, chartManager));
    buttonBar->CreateButton("Gerar In", "Seno", bind(&ChartManager::GenerateSineInput, chartManager));
    buttonBar->CreateButton("Gerar In", "Step", bind(&ChartManager::GenerateStepInput, chartManager));
    buttonBar->CreateButton("Gerar In", "Saw Tooth", bind(&ChartManager::GenerateSawtoothInput, chartManager));
    buttonBar->CreateButton("Carregar In", "input.dct", bind(&ChartManager::LoadInput, chartManager));
    buttonBar->CreateButton("Salvar In", "input.dct", bind(&ChartManager::SaveInput, chartManager));
    buttonBar->CreateButton("Salvar Out", "output.dct", bind(&ChartManager::SaveOutput, chartManager));
}

int main(void)
{
    CV::init(&screenWidth, &screenHeight, "Trabalho 2 - Gustavo Machado de Freitas");

    mouseHandler = new MouseHandler();
    buttonBar = new InteractableBar(barHeight, screenWidth);
    chartManager = new ChartManager(screenWidth, screenHeight);

    StartButtons();

    CV::run();
}
