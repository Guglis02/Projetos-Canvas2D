#ifndef DRAWING_H
#define DRAWING_H

#include "FunctionType.h"
#include "PointsUtils.h"
#include "Vector2.h"
#include "gl_canvas2d.h"
#include <stdio.h>
#include <algorithm>

using namespace std;

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
                    modifyingPointIndex = i;
                    isModifying = true;
                    return true;
                }
            }
            if (DistanceBetweenTwoPoints(mx, my, rotationIndicator.x, rotationIndicator.x) < IndicatorBallRadius)
            {
                printf("\nRota��o iniciada.");
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
                Resize(xInc, yInc);
            }
            else
            {
                Move(Vector2(xInc, yInc));
            }
        }

        void Move(Vector2 inc)
        {
            for (int i = 0; i < elementsCounter; i++)
            {
                this->points[i] += inc;
            }
            for (int i = 0; i < 4; i++)
            {
                this->corners[i] += inc;
            }
            this->rotationIndicator += inc;
        }

        void Resize(int xInc, int yInc)
        {
            Vector2 increment = Vector2(xInc, yInc);
            int opositePointIndex = (modifyingPointIndex + 2) % 4;

            // Reposiciona os dois pontos adjacentes ao que esta sendo movido
            for (int i = -1; i <= 1; i += 2)
            {
                int adjacentPointIndex = (modifyingPointIndex + i + 4) % 4;

                Vector2 adjacentPoint = corners[adjacentPointIndex] - originCorners[adjacentPointIndex];

                corners[adjacentPointIndex] = GetPerpendicularPoint(
                            originCorners[adjacentPointIndex] + adjacentPoint,
                            originCorners[opositePointIndex] + adjacentPoint,
                            corners[modifyingPointIndex] + increment);
            }
            // Reposiciona o ponto movido
            corners[modifyingPointIndex] += increment;

            // Calcula os novos cantos da origem ap�s o movimento
            Vector2 newOriginCorner1 = corners[1] - corners[0];
            Vector2 newOriginCorner3 = corners[3] - corners[0];

            // Calcula a dist�ncia destes novos pontos at� a origem
            Vector2 distanceToOrigin = Vector2 (
                DistanceBetweenTwoPoints(newOriginCorner1, Vector2()),
                DistanceBetweenTwoPoints(newOriginCorner3, Vector2()));

            // Usa a dist�ncia dos pontos e as dimens�es originais do desenho pra calcular a propor��o
            Vector2 proportion = Vector2(
                distanceToOrigin.x / width * 1.0,
                distanceToOrigin.y / height * 1.0);

            // Calcula dist�ncia entre os novos pontos e a outra lateral da caixa de sele��o
            Vector2 distanceToSide = Vector2(
                DistanceBetweenTwoPoints(newOriginCorner1, GetPerpendicularPoint(originCorners[1], originCorners[2], newOriginCorner1)),
                DistanceBetweenTwoPoints(newOriginCorner3, GetPerpendicularPoint(originCorners[2], originCorners[3], newOriginCorner3)));

            // Se os pontos ultrapassaram a outra lateral do desenho, a propor��o � invertida e o desenho � espelhado
            proportion.set(
                distanceToSide.x > max(distanceToOrigin.x, width) ? proportion.x * -1 : proportion.x,
                distanceToSide.y > max(distanceToOrigin.y, height) ? proportion.y * -1 : proportion.y);

            // Atualiza pontos do desenho
            for (int i = 0; i < elementsCounter; i++)
            {
                Vector2 newPoint = Vector2(originPoints[i].x * proportion.x,
                                           originPoints[i].y * proportion.y);

                Vector2 rotate = rotatePoint(newPoint, angle);

                points[i] = rotate + corners[0];
            }

            this->sizeProportion.set(proportion);
            SetRotationPoint();
        }

        void SetSelectionPoints();
        void SetRotationPoint()
        {
            Vector2 selectionBoxTop = (corners[0] + corners[1]) / 2;
            float modifier = sizeProportion.y < 0 ? 1 : -1;

            this->rotationIndicator.x = selectionBoxTop.x + sin(angle);
            this->rotationIndicator.y = selectionBoxTop.y + cos(angle) + modifier * rotationIndicatorOffset;
        }

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

        float GetCenterX(void) { return (this->corners[0].x + this->corners[1].x) / 2; }
        float GetCenterY(void) { return (this->corners[0].y + this->corners[3].y) / 2; }

        float GetHeight(void) { return this->height; }
        float GetWidth(void) { return this->width; }

        int GetElementsCount(void) { return elementsCounter; }

        bool isMoving = false;

    protected:
        FunctionType type;
        int elementsCounter;

        Vector2* points;

        float height;
        float width;

        // Indicadores de que o desenho foi selecionado
        Vector2 corners[4];
        Vector2 rotationIndicator;
        int rotationIndicatorOffset = 20;
        const float IndicatorBallRadius = 7;

        // Coordenadas do desenho relacionadas a origem
        Vector2* originPoints;
        Vector2 originCorners[4];

        // Atributos relacionados a modifica��o do desenho
        bool isModifying = false;
        int modifyingPointIndex = -2;
        float angle = 0.0;
        Vector2 sizeProportion;

        void AddSelectionPoint(int x, int y, int index);

        bool shouldBeFilled = false;

        float r = 0;
        float g = 0;
        float b = 0;

};

#endif // DRAWING_H
