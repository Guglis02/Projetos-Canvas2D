#ifndef DRAWING_H
#define DRAWING_H

#include "FunctionType.h"
#include "PointsUtils.h"
#include "Vector2.h"
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

        void GenerateOriginPoints()
        {
            this->originPoints = new Vector2[this->elementsCounter];

            for (int i = 0; i < elementsCounter; i++)
            {
                originPoints[i] = this->points[i] - this->corners[0];
            }
            for (int i = 0; i < 4; i++)
            {
                originCorners[i] = this->corners[i] - this->corners[0];
            }
        }

        bool CheckMouseInteraction(int mx, int my)
        {
            for (int i = 0; i < 4; i++)
            {
                if (DistanceBetweenTwoPoints(mx, my, corners[i].x, corners[i].y) < IndicatorBallRadius)
                {
                    printf("\nRedimensionamento iniciado no canto %d", i);
                    isModifying = true;
                    return true;
                }
            }
            if (DistanceBetweenTwoPoints(mx, my, rotationIndicator.x, rotationIndicator.x) < IndicatorBallRadius)
            {
                printf("\nRotação iniciada.");
                isModifying = true;
                return true;
            }
            isModifying = false;
            return false;
        }

        void EditDrawing(int xInc, int yInc)
        {
            if (isModifying)
            {
                printf("\nTa editando");
            }
            else
            {
                Move(Vector2(xInc, yInc));
            }

            SetSelectionPoints();
        }

        void Move(Vector2 inc)
        {
            for (int i = 0; i < elementsCounter; i++)
            {
                this->points[i] += inc;
            }
        }

        void SetSelectionPoints();

        void SetColor(float r, float g, float b);
        void SetFillFlag(bool value) { this->shouldBeFilled = value; }

        FunctionType GetType(void) { return this->type; }

        float* GetColor(void) { float* color = new float[3];
                            color[0] = this->r;
                            color[1] = this->g;
                            color[2] = this->b;
                            return color; }

        bool GetFillFlag(void) { return this->shouldBeFilled; }

        float* GetXs(void)
        {
            return Vector2::GetXs(this->points, this->elementsCounter);
        }

        float* GetYs(void)
        {
            return Vector2::GetYs(this->points, this->elementsCounter);
        }

        float GetCenterX(void) { return this->center.x; }
        float GetCenterY(void) { return this->center.y; }

        float GetHeight(void) { return this->height; }
        float GetWidth(void) { return this->width; }

        int GetElementsCount(void) { return elementsCounter; }

        bool isMoving = false;

    protected:
        FunctionType type;
        int elementsCounter;

        Vector2* points;

        Vector2 center;

        float height;
        float width;

        // Indicadores de que o desenho foi selecionado
        Vector2 corners[4];
        Vector2 rotationIndicator;
        const float IndicatorBallRadius = 7;

        // Coordenadas do desenho relacionadas a origem
        Vector2* originPoints;
        Vector2 originCorners[4];

        // Atributos relacionados a modificação do desenho
        bool isModifying = false;
        float angle = 0.0;
        Vector2 sizeProportion;

        void AddSelectionPoint(int x, int y, int index);

        bool shouldBeFilled = false;

        float r = 0;
        float g = 0;
        float b = 0;

};

#endif // DRAWING_H
