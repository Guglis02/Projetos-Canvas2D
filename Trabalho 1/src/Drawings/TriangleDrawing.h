#ifndef TRIANGLEDRAWING_H_INCLUDED
#define TRIANGLEDRAWING_H_INCLUDED

#include "Drawing.h"
#include <algorithm>

/** \brief
Classe responsável pelo desenho em formato de triângulo.
*/
class TriangleDrawing : public Drawing
{
public:
    TriangleDrawing(int x, int y, int width, int height)
    {
        this->type = Triangle;

        this->elementsCounter = 3;

        this->width = width;
        this->height = height;

        this->points = new Vector2[this->elementsCounter];

        this->AddPoint(x, y + height, 0);
        this->AddPoint(x + width * 0.5, y, 1);
        this->AddPoint(x + width, y + height, 2);

        SetSelectionPoints();
        GenerateOriginPoints();
        GenerateAnchor();
    }

    void RenderPrototype(int clickX, int clickY, int currentX, int currentY)
    {
        this->width = (currentX - clickX);
        this->height = (currentY - clickY);

        this->AddPoint(clickX, clickY + max(height, zero), 0);
        this->AddPoint(clickX + width * 0.5, clickY + min(height, zero), 1);
        this->AddPoint(clickX + width, clickY + max(height, zero), 2);

        SetSelectionPoints();
        GenerateOriginPoints();
        GenerateAnchor();
        Render();
    }

    void GenerateAnchor()
    {
        this->anchor = corners[0];
    }

private:
    float zero = 0;
};

#endif // TRIANGLEDRAWING_H_INCLUDED
