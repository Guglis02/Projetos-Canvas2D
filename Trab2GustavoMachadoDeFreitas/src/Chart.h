#ifndef CHART_H_INCLUDED
#define CHART_H_INCLUDED

#include "gl_canvas2d.h"
#include "Vector2.h"
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

class Chart
{
public:
    Chart(float x, float y, float width, float height, const char* name)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->name = name;
    }

    void CreatePoints(vector<double> values)
    {
        points.clear();
        for (int i = 0; i < values.size(); i++)
        {
            points.push_back(Vector2(i, values[i]));
        }
        SetRange();
        normalizedPoints.resize(points.size());
        NormalizePoints();
    }


    void Update(float width, float height)
    {
        if (this->width == width && this->height == height)
        {
            return;
        }

        this->width = width;
        this->height = height;

        NormalizePoints();
    }

    void Render()
    {
        CV::color(1);
        CV::text(x + border, y + height + border, name);
        CV::rect(x, y, x + width, y + height);

        RenderNumericalLabels();

        // Desenha os pontos do gr�fico
        CV::color(0, 0.5, 1);
        for (int i = 1; i < normalizedPoints.size(); i++)
        {
            Vector2 point = normalizedPoints[i];
            Vector2 prevPoint = normalizedPoints[i - 1];
            CV::line(prevPoint.x, prevPoint.y, point.x, point.y);
        }
    }

private:
    const char* name;

    float x, y, width, height;

    const float border = 10;
    const int numericLabelSize = 6;

    // N�o necessariamente esses pontos s�o pontos reais do conjunto
    Vector2 maxPoint;
    Vector2 minPoint;

    Vector2 range;

    vector<Vector2> points;
    vector<Vector2> normalizedPoints;

    void SetRange()
    {
        float xMin = points[0].x;
        float xMax = points[0].x;
        float yMin = points[0].y;
        float yMax = points[0].y;

        for (auto point : points) {
            xMin = min(xMin, point.x);
            xMax = max(xMax, point.x);
            yMin = min(yMin, point.y);
            yMax = max(yMax, point.y);
        }

        this->maxPoint = Vector2(xMax, yMax);
        this->minPoint = Vector2(xMin, yMin);
        this->range = maxPoint - minPoint;
    }

    void NormalizePoints()
    {
        for (int i = 0; i < points.size(); i++)
        {
            normalizedPoints[i].x = x + (points[i].x - minPoint.x) * width / range.x;
            normalizedPoints[i].y = y + (points[i].y - minPoint.y) * height / range.y;
        }
    }

    void RenderNumericalLabels()
    {
        char numericLabel[numericLabelSize];

        sprintf(numericLabel, "%.1f", minPoint.y);
        CV::text(x - border * numericLabelSize, y, numericLabel);
        sprintf(numericLabel, "%.1f", maxPoint.y);
        CV::text(x - border * numericLabelSize, y + height - border, numericLabel);
        sprintf(numericLabel, "%.0f", minPoint.x);
        CV::text(x, y - border * 2, numericLabel);
        sprintf(numericLabel, "%.0f", maxPoint.x);
        CV::text(x + width - border, y - border * 2, numericLabel);
    }
};

#endif // CHART_H_INCLUDED
