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

        this->xs = new float[this->elementsCounter];
        this->ys = new float[this->elementsCounter];

        this->AddPoint(x1, y1, 0);
        this->AddPoint(x2, y1, 1);
        this->AddPoint(x2, y2, 2);
        this->AddPoint(x1, y2, 3);
    }

    Prototype(int x1, int y1, int x2, int y2)
    {
        this->AddPoint(x1, y1, 0);
        this->AddPoint(x2, y1, 1);
        this->AddPoint(x2, y2, 2);
        this->AddPoint(x1, y2, 3);
    }
};


#endif // RECTANGLEDRAWING_H_INCLUDED