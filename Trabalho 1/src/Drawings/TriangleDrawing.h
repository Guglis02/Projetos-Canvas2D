#ifndef TRIANGLEDRAWING_H_INCLUDED
#define TRIANGLEDRAWING_H_INCLUDED

#include "Drawing.h"

class TriangleDrawing : public Drawing
{
    public:
    TriangleDrawing(int x, int y, int width, int height)
    {
        this->type = Triangle;

        this->elementsCounter = 3;

        this->width = width;
        this->height = height;

        this->xs = new float[this->elementsCounter];
        this->ys = new float[this->elementsCounter];

        this->AddPoint(x, y + height, 0);
        this->AddPoint(x + width * 0.5, y, 1);
        this->AddPoint(x + width, y + height, 2);

        SetSelectionPoints();
        GenerateOriginPoints();
    }

    void RenderPrototype(int clickX, int clickY, int currentX, int currentY)
    {
        this->width = (currentX - clickX);
        this->height = (currentY - clickY);

        this->AddPoint(clickX, clickY + height, 0);
        this->AddPoint(clickX + width * 0.5, clickY, 1);
        this->AddPoint(clickX + width, clickY + height, 2);

        SetSelectionPoints();
        GenerateOriginPoints();
        Render();
    }
};

#endif // TRIANGLEDRAWING_H_INCLUDED
