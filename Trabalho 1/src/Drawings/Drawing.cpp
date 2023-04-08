#include "Drawing.h"
#include "FunctionType.h"
#include "gl_canvas2d.h"
#include "PointsUtils.h"
#include <limits>
#include <algorithm>

using namespace std;

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

void Drawing::SetSelectionPoints()
{
    // Variaveis auxiliares para pegar os pontos extremos e montar o indicador de seleção
    float minX = numeric_limits<float>::max();
    float minY = numeric_limits<float>::max();
    float maxX = numeric_limits<float>::lowest();
    float maxY = numeric_limits<float>::lowest();

    for(int i = 0; i < elementsCounter; i++)
    {
        minX = min(xs[i], minX);
        minY = min(ys[i], minY);
        maxX = max(xs[i], maxX);
        maxY = max(ys[i], maxY);
    }

    this->AddSelectionPoint(minX, minY, 0);
    this->AddSelectionPoint(maxX, minY, 1);
    this->AddSelectionPoint(maxX, maxY, 2);
    this->AddSelectionPoint(minX, maxY, 3);

    this->centerX = (maxX + minX) * 0.5;
    this->centerY = (maxY + minY) * 0.5;

    this->rotationIndicatorX = centerX;
    this->rotationIndicatorY = minY - 20;
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
    color(1);
    polygon(cornersXs, cornersYs, 4);
    for (int i = 0; i < 4; i++)
    {
        circleFill(cornersXs[i], cornersYs[i], IndicatorBallRadius, 10);
    }

    color(1);
    circleFill(rotationIndicatorX, rotationIndicatorY, IndicatorBallRadius, 10);
}

bool Drawing::CheckMouseClick(int mx, int my)
{
    return pnpoly(this->elementsCounter,
                  this->xs,
                  this->ys,
                  mx, my);
}



