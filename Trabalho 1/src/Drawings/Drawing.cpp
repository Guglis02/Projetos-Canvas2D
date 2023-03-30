#include "Drawing.h"
#include "FunctionType.h"
#include "gl_canvas2d.h"


void Drawing::Render()
{
    color(this->r, this->g, this->b);
    if (shouldBeFilled)
    {
        polygonFill(this->xs, this->ys, this->elementsCounter);
    } else
    {
        polygon(this->xs, this->ys, this->elementsCounter);
    }
}

void Drawing::SetColor(int r, int g, int b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

void Drawing::SetFillable(bool value)
{
    this->shouldBeFilled = value;
}

void Drawing::AddPoint(int x, int y, int index)
{
    this->xs[index] = x;
    this->ys[index] = y;
}
