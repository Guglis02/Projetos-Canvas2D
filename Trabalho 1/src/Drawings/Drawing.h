#ifndef DRAWING_H
#define DRAWING_H

#include "FunctionType.h"

class Drawing
{
    public:
        void Render();
        void SetColor(float r, float g, float b);
        void SetFillable(bool value);
        void AddPoint(int x, int y, int index);

    protected:
        FunctionType type;
        int elementsCounter;

        float *xs;
        float *ys;

        bool shouldBeFilled = false;

        int r = 0;
        int g = 0;
        int b = 0;
};

#endif // DRAWING_H
