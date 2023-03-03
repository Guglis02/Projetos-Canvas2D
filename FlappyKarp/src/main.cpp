/*********************************************************************
// Flappy Karp
// Autor: Gustavo Machado de Freitas
//        03/2023
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "gl_canvas2d.h"
#include "GameManager.h"


GameManager* gameManager = NULL;

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis
//globais que podem ser setadas pelo metodo keyboard()
void render()
{
    gameManager->Update();
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    return;
}

void keyboard(int key)
{
    gameManager->HandleInputs(key);
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
    return;
}


int main(void)
{
    int screenWidth = 500, screenHeight = 500; //largura e altura inicial da tela. Alteram com o redimensionamento de tela.
    init(&screenWidth, &screenHeight, "Flappy Karp");
    gameManager = new GameManager(screenWidth, screenHeight);
    run();
}
