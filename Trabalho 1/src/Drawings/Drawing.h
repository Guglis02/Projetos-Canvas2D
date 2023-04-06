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

        void Resize(float xFactor, float yFactor)
        {
            float tMatrix[3][3] =
            {
                {xFactor, 0, 0},
                {0, yFactor, 0},
                {0, 0, 1}
            };

            for (int i = 0; i < this->elementsCounter; i++)
            {
                float point[3] = {this->xs[i], this->ys[i]};

                float newPoint[3] = {0, 0, 0};
                for (int j = 0; j < 3; j++)
                {
                    for (int k = 0; k < 3; k++)
                    {
                        newPoint[j] += tMatrix[j][k] * point[k];
                    }
                }
            }

            SetSelectionPoints();
        }

        void SetSelectionPoints();

        void SetColor(float r, float g, float b);
        void SetFillFlag(bool value) { this->shouldBeFilled = value; }

        FunctionType GetType() { return this->type; }

        float* GetXs() { return this->xs; }

        float* GetYs() { return this->ys; }

        float* GetColor() { float* color = new float[3];
                            color[0] = this->r;
                            color[1] = this->g;
                            color[2] = this->b;
                            return color; }

        bool GetFillFlag() { return this->shouldBeFilled; }

        float GetCenterX() { return this->centerX; }
        float GetCenterY() { return this->centerY; }

        float GetHeight() { return this->height; }
        float GetWidth() { return this->width; }

        int GetElementsCount() { return elementsCounter; }

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
