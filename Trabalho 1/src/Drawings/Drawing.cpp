#include "Drawing.h"
#include "FunctionType.h"
#include "gl_canvas2d.h"
#include "PointsUtils.h"
#include "Vector2.h"
#include <limits>
#include <algorithm>

using namespace std;

void Drawing::Render()
{
    color(this->r, this->g, this->b);
    if (shouldBeFilled)
    {
        polygonFill(Vector2::GetXs(this->points, elementsCounter),
                    Vector2::GetYs(this->points, elementsCounter),
                    this->elementsCounter);
    } else
    {
        polygon(Vector2::GetXs(this->points, elementsCounter),
                Vector2::GetYs(this->points, elementsCounter),
                this->elementsCounter);
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
        minX = min(points[i].x, minX);
        minY = min(points[i].y, minY);
        maxX = max(points[i].x, maxX);
        maxY = max(points[i].y, maxY);
    }

    this->AddSelectionPoint(minX, minY, 0);
    this->AddSelectionPoint(maxX, minY, 1);
    this->AddSelectionPoint(maxX, maxY, 2);
    this->AddSelectionPoint(minX, maxY, 3);

    this->height = maxY - minY;
    this->width = maxX - minX;

    SetRotationPoint();
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
    this->points[index] = Vector2(x, y);
}

void Drawing::AddSelectionPoint(int x, int y, int index)
{
    this->corners[index] = Vector2(x, y);
}

void Drawing::RenderSelectionIndicators()
{
    color(1);
    polygon(Vector2::GetXs(this->corners, 4),
            Vector2::GetYs(this->corners, 4),
            4);
    for (int i = 0; i < 4; i++)
    {
        circleFill(corners[i].x, corners[i].y, IndicatorBallRadius, 10);
    }

    color(1);
    circleFill(rotationIndicator.x, rotationIndicator.y, IndicatorBallRadius, 10);
}

bool Drawing::CheckMouseClick(int mx, int my)
{
    return pnpoly(this->elementsCounter,
                  Vector2::GetXs(this->corners, elementsCounter),
                  Vector2::GetYs(this->corners, elementsCounter),
                  mx, my);
}



