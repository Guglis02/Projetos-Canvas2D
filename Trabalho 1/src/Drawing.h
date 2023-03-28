#ifndef DRAWING_H
#define DRAWING_H

#include "FunctionType.h"

class Drawing
{
    public:
        Drawing(int x1, int y1, int x2, int y2, FunctionType type);
        Drawing(int x, int y, FunctionType type);

        void Update();
    private:
        int x1;
        int y1;
        int x2;
        int y2;

        FunctionType type;
};

#endif // DRAWING_H
