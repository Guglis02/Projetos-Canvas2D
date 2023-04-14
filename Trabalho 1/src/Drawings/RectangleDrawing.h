#ifndef RECTANGLEDRAWING_H_INCLUDED
#define RECTANGLEDRAWING_H_INCLUDED

#include "Drawing.h"

class RectangleDrawing : public Drawing
{
    public:

    RectangleDrawing(int x1, int y1, int x2, int y2)
    {
        this->type = Rect;

        this->elementsCounter = 4;

        this->points = new Vector2[this->elementsCounter];

        this->AddPoint(x1, y1, 0);
        this->AddPoint(x2, y1, 1);
        this->AddPoint(x2, y2, 2);
        this->AddPoint(x1, y2, 3);

        SetSelectionPoints();
        GenerateAnchor();
        GenerateOriginPoints();
    }

    void RenderPrototype(int clickX, int clickY, int currentX, int currentY)
    {
        this->AddPoint(clickX, clickY, 0);
        this->AddPoint(currentX, clickY, 1);
        this->AddPoint(currentX, currentY, 2);
        this->AddPoint(clickX, currentY, 3);

        SetSelectionPoints();
        GenerateAnchor();
        GenerateOriginPoints();
        Render();
    }

    void GenerateAnchor()
    {
        this->anchor = corners[0];
    }
};


#endif // RECTANGLEDRAWING_H_INCLUDED
