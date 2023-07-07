#ifndef __CANVAS_2D__H__
#define __CANVAS_2D__H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <vector>

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include "./Utils/VectorHomo3d.h"

#define PI_2 6.28318530717
#define PI   3.14159265359

#define Y_CANVAS_CRESCE_PARA_CIMA 1

using namespace std;

class CV //classe Canvas2D
{
public:
    //funcoes para desenho de ponto e linha
    static void point(float x, float y); //coordeandas do ponto
    static void point(VectorHomo3d pos); //coordeandas do ponto

    static void line( float x1, float y1, float x2, float y2 ); //coordenadas da linha x1, y1, x2, y2
    static void line( VectorHomo3d p1, VectorHomo3d p2 )
    {
        line(p1.x, p1.y, p2.x, p2.y);
    }
    static void perspectiveLine( VectorHomo3d p1, VectorHomo3d p2, float d)
    {
        if (p1.z <= 0 || p2.z <= 0)
        {
            return;
        }

        line(d * p1.x/p1.z, d * p1.y/p1.z, d * p2.x/p2.z, d * p2.y/p2.z);
    }

    //desenha um retangulo alinhado nos eixos x e y
    static void rect( float x1, float y1, float x2, float y2 ); //coordenadas do retangulo x1, y1, x2, y2
    static void rect( VectorHomo3d p1, VectorHomo3d p2 ); //coordenadas do retangulo (p1, p2) - TO DO

    static void rectFill( float x1, float y1, float x2, float y2 ); //coordenadas do retangulo x1, y1, x2, y2
    static void rectFill( VectorHomo3d p1, VectorHomo3d p2 ); //coordenadas do retangulo (p1, p2)

    //desenha um poligono CONVEXO. Para um retangulo, deve-se passar 4 vertices
    static void polygon(float vx[], float vy[], int n_elems);
    static void polygon(vector<VectorHomo3d> v)
    {
        float vx[v.size()];
        float vy[v.size()];
        for (unsigned int i = 0; i < v.size(); i++)
        {
            vx[i] = v[i].x;
            vy[i] = v[i].y;
        }
        polygon(vx, vy, v.size());
    }
    static void polygonFill(float vx[], float vy[], int n_elems);

    //centro e raio do circulo
    static void circle( float x, float y, float radius, int div );
    static void circle( VectorHomo3d pos, float radius, int div )
    {
        circle(pos.x, pos.y, radius, div);
    }

    static void circleFill( float x, float y, float radius, int div );
    static void circleFill( VectorHomo3d pos, float radius, int div )
    {
        circleFill(pos.x, pos.y, radius, div);
    }

    static void ellipsisFill( float x, float y, float radiusX, float radiusY, int div );
    static void ellipsisFill( float x, float y, float radiusX, float radiusY, int div, float angle );

    //especifica a cor de desenho e de limpeza de tela
    static void color(float r, float g, float b);
    static void color(float r, float g, float b, float alpha);
    static void color(int index);

    static void clear(float r, float g, float b);

    //desenha texto na coordenada (x,y)
    static void text(float x, float y, const char *t);
    static void text(VectorHomo3d pos, const char *t)
    {
        text(pos.x, pos.y, t);
    }

    //coordenada de offset para desenho de objetos.
    static void translate(float x, float y);
    static void translate(VectorHomo3d pos);

    //funcao de inicializacao da Canvas2D. Recebe a largura, altura, e um titulo para a janela
    static void init(int *w, int *h, const char *title);

    //funcao para executar a Canvas2D
    static void run();
};

#endif
