#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <stdio.h>
#include <cstdlib>

#include "gl_canvas2d.h"
#include "GameManager.h"
#include "MouseHandler.h"

// using namespace std;

// Largura e altura inicial da tela. Alteram com o redimensionamento de tela.
int screenWidth = 940, screenHeight = 940;

GameManager* gameManager = NULL;

// Funcao chamada todo frame
void render()
{
    for (int i = 0; i <= screenHeight; i++)
    {
        for (int j = 0; j <= screenWidth; j++)
        {
            int colorIndex = (i + j ) % 11;
            CV::color(colorIndex);
            CV::point(i, j);
        }
    }
    gameManager->Update(screenWidth, screenHeight);
}

// Funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);
    //gameManager->mouseHandler->Update(button, state, wheel, direction, x, y);
}

// Funcao chamada toda vez que uma tecla for pressionada
void keyboard(int key)
{
    gameManager->KeyPressed(key);
}

// Funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
    gameManager->KeyReleased(key);
}

int main(void)
{
    CV::init(&screenWidth, &screenHeight, "Trabalho 3 - Gustavo Machado de Freitas");

    gameManager = new GameManager();

    CV::run();
}
