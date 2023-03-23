#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "gl_canvas2d.h"
#include "7SegmentsDisplay.h"

void Draw7SegmentsDisplay(int x, int y, int tam, int gap, int num)
{
    float halfGap = gap * 0.5;
    float side = tam * 6 + halfGap;

    float horizontalBarXs[6] = {0, tam, tam*5, tam*6, tam*5, tam};
    float horizontalBarYs[6] = {0, tam, tam, 0, -tam, -tam};

    float verticalBarXs[6] = {0, -tam, -tam, 0, tam, tam};
    float verticalBarYs[6] = {0, tam, tam*5, tam*6, tam*5, tam};

    int arrBits[] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B};

    if ( (arrBits[num] >> 6 & 1) == 1)
    {
        // A
        translate(x, y);
        polygonFill(horizontalBarXs, horizontalBarYs, 6);
    }
    if ( (arrBits[num] >> 5 & 1) == 1)
    {
        // B
        translate(x + side, y + halfGap);
        polygonFill(verticalBarXs, verticalBarYs, 6);
    }
    if ( (arrBits[num] >> 4 & 1) == 1)
    {
        // C
        translate(x + side, y + side + gap);
        polygonFill(verticalBarXs, verticalBarYs, 6);
    }
    if ( (arrBits[num] >> 3 & 1) == 1)
    {
        // D
        translate(x, y + 2 * side + gap);
        polygonFill(horizontalBarXs, horizontalBarYs, 6);
    }
    if ( (arrBits[num] >> 2 & 1) == 1)
    {
        // E
        translate(x, y + side + gap);
        polygonFill(verticalBarXs, verticalBarYs, 6);
    }
    if ( (arrBits[num] >> 1 & 1) == 1)
    {
        // F
        translate(x, y + halfGap);
        polygonFill(verticalBarXs, verticalBarYs, 6);
    }
    if ( (arrBits[num] >> 0 & 1) == 1)
    {
        // G
        translate(x, y + side + halfGap);
        polygonFill(horizontalBarXs, horizontalBarYs, 6);
    }
}


