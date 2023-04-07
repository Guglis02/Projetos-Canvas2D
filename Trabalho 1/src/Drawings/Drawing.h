#ifndef DRAWING_H
#define DRAWING_H

#include "FunctionType.h"
#include <stdio.h>

class Drawing
{
    public:
        void Render();
        void RenderSelectionIndicators();
        virtual void RenderPrototype(int clickX, int clickY, int currentX, int currentY){};
        void SwitchFillable();
        void AddPoint(int x, int y, int index);
        bool CheckMouseClick(int mx, int my);

        void Move(int xInc, int yInc)
        {
            for (int i = 0; i < elementsCounter; i++)
            {
                this->xs[i] += xInc;
                this->ys[i] += yInc;
            }

            SetSelectionPoints();
        }

        void SetSelectionPoints();

        void SetColor(float r, float g, float b);
        void SetFillFlag(bool value) { this->shouldBeFilled = value; }

        FunctionType GetType(void) { return this->type; }

        float* GetXs(void) { return this->xs; }

        float* GetYs(void) { return this->ys; }

        float* GetColor(void) { float* color = new float[3];
                            color[0] = this->r;
                            color[1] = this->g;
                            color[2] = this->b;
                            return color; }

        bool GetFillFlag(void) { return this->shouldBeFilled; }

        float GetCenterX(void) { return this->centerX; }
        float GetCenterY(void) { return this->centerY; }

        float GetHeight(void) { return this->height; }
        float GetWidth(void) { return this->width; }

        int GetElementsCount(void) { return elementsCounter; }

        bool isMoving = false;

    protected:
        FunctionType type;
        int elementsCounter;

        float* xs;
        float* ys;

        float centerX;
        float centerY;

        float height;
        float width;

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
