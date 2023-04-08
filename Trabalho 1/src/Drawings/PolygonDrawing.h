#ifndef POLYGONDRAWING_H_INCLUDED
#define POLYGONDRAWING_H_INCLUDED

#include "Drawing.h"


class PolygonDrawing : public Drawing
{
    public:
        PolygonDrawing(int xs[], int ys[], int elementsCounter)
        {
            this->type = Poly;

            this->elementsCounter = elementsCounter;

            this->xs = new float[this->elementsCounter];
            this->ys = new float[this->elementsCounter];

            for(int i = 0; i < elementsCounter; i++)
            {
                this->AddPoint(xs[i], ys[i], i);
            }

            SetSelectionPoints();
            GenerateOriginPoints();
        }
};

#endif // POLYGONDRAWING_H_INCLUDED
