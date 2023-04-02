#include "Drawing.h"
#include "FunctionType.h"
#include "gl_canvas2d.h"
#include "PointsUtils.h"

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

void Drawing::SetColor(float r, float g, float b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

void Drawing::SwitchFillable()
{
    this->shouldBeFilled = !this->shouldBeFilled;
}

void Drawing::AddPoint(int x, int y, int index)
{
    this->xs[index] = x;
    this->ys[index] = y;
}

void Drawing::AddSelectionPoint(int x, int y, int index)
{
    this->cornersXs[index] = x;
    this->cornersYs[index] = y;
}

void Drawing::RenderSelectionIndicators()
{
    color(0);
    polygon(cornersXs, cornersYs, 4);
    for (int i = 0; i < 4; i++)
    {
        circleFill(cornersXs[i], cornersYs[i], indicatorBallRadius, 10);
    }
}

bool Drawing::CheckMouseClick(int mx, int my)
{
    return pnpoly(this->elementsCounter,
                  this->xs,
                  this->ys,
                  mx, my);
}



