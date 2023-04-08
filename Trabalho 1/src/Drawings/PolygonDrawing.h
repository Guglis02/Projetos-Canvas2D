#ifndef POLYGONDRAWING_H_INCLUDED
#define POLYGONDRAWING_H_INCLUDED

#include "Drawing.h"


class PolygonDrawing : public Drawing
{
    public:
        PolygonDrawing(float xs[], float ys[], int elementsCounter)
        {
            this->type = Poly;

            this->elementsCounter = elementsCounter;

            this->points = new Vector2[this->elementsCounter];

            for(int i = 0; i < elementsCounter; i++)
            {
                this->AddPoint(xs[i], ys[i], i);
            }

            SetSelectionPoints();
            GenerateOriginPoints();
        }
};

#endif // POLYGONDRAWING_H_INCLUDED
