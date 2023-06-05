#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <stdio.h>
#include <cstdlib>
#include <ctime>

#include "gl_canvas2d.h"
#include "Vector2.h"

using namespace std;

// Largura e altura inicial da tela. Alteram com o redimensionamento de tela.
int screenWidth = 1400, screenHeight = 700;

float degreeToRadians(float degree)
{
    return degree * PI / 180.0;
}

float renderGear(float angIni)
{
    float ang = 0;
    float raio1 = 100;
    float raio2 = 120;
    float raio = raio1;
    float passo = (2.0 * PI)/80;

    float lastX, lastY;
    lastX = raio2 * cos(2 * PI - passo + angIni);
    lastY = raio2 * sin(2 * PI - passo + angIni);

    for( ang = 0; ang <= 2 * PI ; ang += passo)
    {
        float x1 = raio * cos(ang + angIni);
        float y1 = raio * sin (ang + angIni);

        ang += passo;

        float x2 = raio * cos(ang + angIni);
        float y2 = raio * sin (ang + angIni);
        
        CV::line(x1, y1, x2, y2);
        CV::line(x1, y1, lastX, lastY);

        lastX = x2;
        lastY = y2;

        if( raio == raio1)
            raio = raio2;
        else
            raio = raio1;
    }
}

void renderSpiral(float angIni)
{
    float ang = 0;
    float raio = 0;
    for(int i = 0; i < 1000; i++)
    {
         float x = raio * cos(ang + angIni);
         float y = raio * sin(ang + angIni);
         CV::point(x, y);
         raio +=0.1;
         ang -= 0.01;
    }
    angIni -= 0.01;
}

void renderHourClock()
{
    float ang = 0;
    float raio = 200;
    float passo = (2.0 * PI)/12;
    float offSet = PI * 0.5;

    for (int h = 0; h < 12; i++)
    {
        float x = raio * cos(ang + offSet);
        float y = raio * sin(ang + offSet);
    }

}

float angIni = 0; //ângulo inicial que eh aumentado a cada render

void render()
{
    CV::translate(screenWidth >> 1, screenHeight >> 1);
    CV::color(1);

    renderGear(angIni);
    renderSpiral(angIni);
    renderHourClock();

    angIni -= 0.001;


}


// Funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y) {}

// Funcao chamada toda vez que uma tecla for pressionada
void keyboard(int key) {}
// Funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key) {}

int main(void)
{
    CV::init(&screenWidth, &screenHeight, "Engrenagem");

    CV::run();
}
