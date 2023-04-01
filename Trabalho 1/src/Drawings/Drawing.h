#ifndef DRAWING_H
#define DRAWING_H

#include "FunctionType.h"

class Drawing
{
    public:
        void Render();
        void SetColor(float r, float g, float b);
        void SwitchFillable();
        void AddPoint(int x, int y, int index);
        void RenderSelectionIndicators();
        bool CheckMouseClick(int mx, int my);
    protected:
        FunctionType type;
        int elementsCounter;

        float* xs;
        float* ys;

        // Indicadores de que o desenho foi selecionado
        float cornersXs[4];
        float cornersYs[4];
        float rotationIndicatorX;
        float rotationIndicatorY;
        const float indicatorBallRadius = 5;

        void AddSelectionPoint(int x, int y, int index);

        bool shouldBeFilled = false;

        float r = 0;
        float g = 0;
        float b = 0;
};

#endif // DRAWING_H
