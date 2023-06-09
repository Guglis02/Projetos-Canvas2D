#ifndef VECTORARTS_H_INCLUDED
#define VECTORARTS_H_INCLUDED

#include "../gl_canvas2d.h"
#include "VectorHomo.h"

// Arquivo contendo desenhos vetoriais

static void DrawMillenniumFalcon(VectorHomo position)
{
    CV::translate(position);

    CV::color(0.5f, 0.5f, 0.5f);

    float lx[] = {-32, -20, -5, -5};
    float ly[] = {5, 50, 50, 5};
    CV::polygonFill(lx, ly, 4);

    float rx[] = {32, 20, 5, 5};
    float ry[] = {5, 50, 50, 5};
    CV::polygonFill(rx, ry, 4);

    CV::color(0.6f, 0.6f, 0.6f);
    CV::circleFill(0, 0, 32, 32);

    CV::color(0.7f, 0.7f, 0.7f);
    CV::circleFill(0, 0, 10, 10);
    CV::rectFill(-32, 5, 32, -5);

    CV::rectFill(32, 32, 40, 20);
    float bx[] = {0, 32, 40, 5};
    float by[] = {0, 20, 20, -5};
    CV::polygonFill(bx, by, 4);

    CV::color(0, 0, 0);
    CV::circle(0, 0, 10, 10);

    CV::translate(0, 0);
}

static void DrawMillenniumFalconWireframe(VectorHomo position)
{
    CV::translate(position);

    CV::color(0.5f, 0.5f, 0.5f);

    float lx[] = {-32, -20, -5, -5};
    float ly[] = {5, 50, 50, 5};
    CV::polygon(lx, ly, 4);

    float rx[] = {32, 20, 5, 5};
    float ry[] = {5, 50, 50, 5};
    CV::polygon(rx, ry, 4);

    CV::color(0.6f, 0.6f, 0.6f);
    CV::circle(0, 0, 32, 32);

    CV::color(0.7f, 0.7f, 0.7f);
    CV::circle(0, 0, 10, 10);
    CV::rect(-32, 5, 32, -5);

    CV::rect(32, 32, 40, 20);
    float bx[] = {0, 32, 40, 5};
    float by[] = {0, 20, 20, -5};
    CV::polygon(bx, by, 4);

    CV::color(0, 0, 0);
    CV::circle(0, 0, 10, 10);

    CV::translate(0, 0);
}

static void DrawTieFighter(VectorHomo position)
{
    CV::translate(position);

    CV::color(0.5f, 0.5f, 0.5f);

    // Asas
    CV::rectFill(VectorHomo(-32, 32),
                 VectorHomo(-28, -32));
    CV::rectFill(VectorHomo(32, 32),
                 VectorHomo(28, -32));

    // Hastes
    CV::rectFill(VectorHomo(-32, 5),
                 VectorHomo(0, -5));
    CV::rectFill(VectorHomo(32, 5),
                 VectorHomo(0, -5));

    CV::color(0.6f, 0.6f, 0.6f);
    CV::circleFill(0, 0, 16, 16);

    CV::translate(0, 0);
}

static void DrawTieBomber(VectorHomo position)
{
    CV::translate(position);

    CV::color(0.6f, 0.6f, 0.6f);

    // Hastes
    CV::rectFill(VectorHomo(-32, 5),
                 VectorHomo(32, -5));

    // Canhões
    CV::elipsisFill(-10, 0, 10, 16, 16);
    CV::color(0.5f, 0.5f, 0.5f);
    CV::elipsisFill(10, 0, 10, 14, 16);

    CV::color(0.7f, 0.7f, 0.7f);

    // Asas
    float lx[] = {-32, -20, -20, -32};
    float ly[] = {32, 20, -20, -32};
    CV::polygonFill(lx, ly, 4);

    float rx[] = {32, 20, 20, 32};
    float ry[] = {32, 20, -20, -32};
    CV::polygonFill(rx, ry, 4);

    CV::translate(0, 0);
}

#endif // VECTORARTS_H_INCLUDED