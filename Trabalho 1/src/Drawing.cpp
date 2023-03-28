#include "Drawing.h"
#include "FunctionType.h"
#include "gl_canvas2d.h"

Drawing::Drawing(int x1, int y1, int x2, int y2, FunctionType type)
{
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;

    this->type = type;
}

Drawing::Drawing(int x, int y, int radius, FunctionType type)
{
    this->x1 = x;
    this->y1 = y;
    this->radius = radius;

    this->type = type;
}

void Drawing::Update()
{
    color(3);
    switch(type)
    {
        case Rect:
            rect(x1, y1, x2, y2);
            break;
        case Circle:
            circle(x1, y1, radius, 32);
            break;
        default:
            break;
    }
}
