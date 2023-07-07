#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include "gl_canvas2d.h"
#include "./Utils/GlobalConsts.h"
#include "./EnginesManager.h"

using namespace std;

// Largura e altura inicial da tela. Alteram com o redimensionamento de tela.
int screenWidth = ConstScreenWidth, screenHeight = ConstScreenHeight;
EnginesManager *enginesManager;

void render(void)
{
    FpsController::getInstance().updateFrames();

    enginesManager->Render();
}

// Funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    // printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);
}

// Funcao chamada toda vez que uma tecla for pressionada
void keyboard(int key)
{
    enginesManager->ProcessKey(key);
}

// Funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
}

int main(void)
{
    enginesManager = new EnginesManager();

    CV::init(&screenWidth, &screenHeight, "Trabalho 4 - Gustavo Machado de Freitas");
    CV::run();
}
