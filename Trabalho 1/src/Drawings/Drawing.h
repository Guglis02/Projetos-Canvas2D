#ifndef DRAWING_H
#define DRAWING_H

#include "FunctionType.h"
#include "PointsUtils.h"
#include "Vector2.h"
#include "gl_canvas2d.h"
#include <stdio.h>
#include <algorithm>
#include <iostream>

using namespace std;

class Drawing
{
    public:
        //
        // Render
        //
        void Render(void)
        {
            color(this->r, this->g, this->b);
            if (shouldBeFilled)
            {
                polygonFill(Vector2::GetXs(this->points, elementsCounter),
                            Vector2::GetYs(this->points, elementsCounter),
                            this->elementsCounter);
            } else
            {
                polygon(Vector2::GetXs(this->points, elementsCounter),
                        Vector2::GetYs(this->points, elementsCounter),
                        this->elementsCounter);
            }
            color(2);
        }
        void RenderSelectionIndicators(void)
        {
            color(1);
            polygon(Vector2::GetXs(this->corners, 4),
                    Vector2::GetYs(this->corners, 4),
                    4);
            for (int i = 0; i < 4; i++)
            {
                circleFill(corners[i].x, corners[i].y, IndicatorBallRadius, 10);
            }

            color(1);
            circleFill(rotationIndicator.x, rotationIndicator.y, IndicatorBallRadius, 10);
        }
        virtual void RenderPrototype(int clickX, int clickY, int currentX, int currentY){};

        //
        // Mouse
        //
        bool CheckMouseClick(int mx, int my)
        {
            return pnpoly(this->elementsCounter,
                          Vector2::GetXs(this->points, elementsCounter),
                          Vector2::GetYs(this->points, elementsCounter),
                          mx, my);
        }
        bool CheckMouseInteraction(int mx, int my)
        {
            for (int i = 0; i < 4; i++)
            {
                if (DistanceBetweenTwoPoints(mx, my, corners[i].x, corners[i].y) < IndicatorBallRadius)
                {
                    modifyingPointIndex = i;
                    isResizing = true;
                    isRotating = false;
                    return true;
                }
            }
            if (DistanceBetweenTwoPoints(mx, my, this->rotationIndicator.x, this->rotationIndicator.y) < IndicatorBallRadius)
            {
                isRotating = true;
                isResizing = false;
                return true;
            }
            isResizing = false;
            isRotating = false;
            return false;
        }

        //
        // Edição de desenho (alguns métodos recebem input do mouse, outros recebem input do arquivo de figuras salvas)
        //
        void EditDrawing(int mx, int my, int xInc, int yInc)
        {
            if (isResizing)
            {
                Resize(xInc, yInc);
            }
            else if (isRotating)
            {
                Rotate(mx, my);
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
            this->anchor += inc;
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

            // Calcula os novos cantos da origem após o movimento
            Vector2 newOriginCorner1 = corners[1] - corners[0];
            Vector2 newOriginCorner3 = corners[3] - corners[0];

            // Calcula a distância destes novos pontos até a origem
            Vector2 distanceToOrigin = Vector2 (
                DistanceBetweenTwoPoints(newOriginCorner1, Vector2()),
                DistanceBetweenTwoPoints(newOriginCorner3, Vector2()));

            // Usa a distância dos pontos e as dimensões originais do desenho pra calcular a proporção do redimensionamento
            Vector2 proportion = Vector2(
                distanceToOrigin.x / width * 1.0,
                distanceToOrigin.y / height * 1.0);

            // Calcula distância entre os novos pontos e os pontos originais
            Vector2 distanceMoved = Vector2(
                DistanceBetweenTwoPoints(newOriginCorner1, GetPerpendicularPoint(originCorners[1], originCorners[2], newOriginCorner1)),
                DistanceBetweenTwoPoints(newOriginCorner3, GetPerpendicularPoint(originCorners[2], originCorners[3], newOriginCorner3)));

            // Se a distância dos novos pontos de controle em relação a suas posições originais
            // for maior do que a distância deles a origem, ou maior do que sua largura, implica que
            // o desenho foi invertido, logo, a proporção é invertida e o desenho é espelhado
            proportion.set(
                distanceMoved.x > max(distanceToOrigin.x, width) ? proportion.x * -1 : proportion.x,
                distanceMoved.y > max(distanceToOrigin.y, height) ? proportion.y * -1 : proportion.y);

            // Atualiza pontos do desenho
            for (int i = 0; i < elementsCounter; i++)
            {
                Vector2 newPoint = Vector2(originPoints[i].x * proportion.x,
                                           originPoints[i].y * proportion.y);

                Vector2 rotated = RotatePoint(newPoint, angle);

                points[i] = rotated + GenerateAndReturnAnchor();
            }

            this->sizeProportion.set(proportion);
            SetRotationPoint();
            GenerateAnchor();
        }

        void LoadProportion(Vector2 proportion)
        {
            Vector2 tempAnchor = GenerateAndReturnAnchor();
            // Atualiza pontos do desenho
            for (int i = 0; i < 4; i++)
            {
                Vector2 newPoint = Vector2(originCorners[i].x * proportion.x,
                                           originCorners[i].y * proportion.y);

                corners[i] = newPoint + tempAnchor;
            }
            for (int i = 0; i < elementsCounter; i++)
            {
                Vector2 newPoint = Vector2(originPoints[i].x * proportion.x,
                                           originPoints[i].y * proportion.y);

                points[i] = newPoint + GenerateAndReturnAnchor();
            }

            this->sizeProportion.set(proportion);
            SetRotationPoint();
            GenerateAnchor();
        }

        void Rotate(int mx, int my)
        {
            Vector2 originToCenter = Vector2((corners[0] + corners[2]) / 2.0);

            float angle = GetAngleBetweenPoints(rotationIndicator - originToCenter,
                                                Vector2(mx, my) - originToCenter);

            ApplyAngle(angle);
        }

        void ApplyAngle(float angle)
        {
            Vector2 originToCenter = Vector2((corners[0] + corners[2]) / 2.0);

            // Rotaciona o indicador de rotação na origem

            Vector2 originRotationPoint = rotationIndicator - originToCenter;

            Vector2 rotated = RotatePoint(originRotationPoint, angle);

            rotationIndicator = rotated + originToCenter;

            // Rotaciona os pontos do desenho na origem
            for (int i = 0; i < elementsCounter; i++)
            {
                rotated = RotatePoint(points[i] - originToCenter, angle);
                points[i] = rotated + originToCenter;
            }
            // Rotaciona os indicadores de seleção na origem
            for (int i = 0; i < 4; i++)
            {
                rotated = RotatePoint(corners[i] - originToCenter, angle);
                corners[i] = rotated + originToCenter;

                rotated = RotatePoint(originCorners[i], angle);
                originCorners[i] = rotated;
            }

            this->angle += angle;
        }

        //
        // Setters
        //
        void SetSelectionPoints(void)
        {
            // Variaveis auxiliares para pegar os pontos extremos e montar o indicador de seleção
            float minX = numeric_limits<float>::max();
            float minY = numeric_limits<float>::max();
            float maxX = numeric_limits<float>::lowest();
            float maxY = numeric_limits<float>::lowest();

            for(int i = 0; i < elementsCounter; i++)
            {
                minX = min(points[i].x, minX);
                minY = min(points[i].y, minY);
                maxX = max(points[i].x, maxX);
                maxY = max(points[i].y, maxY);
            }

            this->AddSelectionPoint(minX, minY, 0);
            this->AddSelectionPoint(maxX, minY, 1);
            this->AddSelectionPoint(maxX, maxY, 2);
            this->AddSelectionPoint(minX, maxY, 3);

            this->height = maxY - minY;
            this->width = maxX - minX;

            this->SetRotationPoint();
        }
        void SetRotationPoint(void)
        {
            Vector2 selectionBoxTop = (this->corners[0] + this->corners[1]) * 0.5;
            float modifier = sizeProportion.y < 0 ? 1 : -1;

            this->rotationIndicator.x = selectionBoxTop.x + sin(angle) * modifier * -rotationIndicatorOffset;
            this->rotationIndicator.y = selectionBoxTop.y + cos(angle) * modifier * rotationIndicatorOffset;
        }
        void SetColor(float r, float g, float b)
        {
            this->r = r;
            this->g = g;
            this->b = b;
        }
        void SetFillFlag(bool value) { this->shouldBeFilled = value; }
        void SwitchFillable(void)
        {
            this->shouldBeFilled = !this->shouldBeFilled;
        }
        //
        // Getters
        //
        FunctionType GetType(void) { return this->type; }
        float* GetColor(void)
        {
            float* color = new float[3];
            color[0] = this->r;
            color[1] = this->g;
            color[2] = this->b;
            return color;
        }
        bool GetFillFlag(void) { return this->shouldBeFilled; }
        float GetAngle(void) { return this->angle; }
        Vector2 GetProportion(void) { return this->sizeProportion; }
        Vector2* GetOriginPoints(void)
        {
            return this->originPoints;
        }
        float GetHeight(void) { return this->height; }
        float GetWidth(void) { return this->width; }
        Vector2 GetAnchor(void) { return this->anchor; }
        int GetElementsCount(void) { return elementsCounter; }
        Vector2 GetGlobalCenter(void) { (this->corners[0] + (this->corners[2] - this->corners[0]) * 0.5); }

        bool isMoving = false;

    protected:
        FunctionType type;
        int elementsCounter;

        Vector2* points;

        // Atributos que serão usados para restaurar o desenho após o load
        Vector2 anchor;
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

        // Atributos relacionados a modificação do desenho
        bool isResizing = false;
        bool isRotating = false;
        int modifyingPointIndex = -1;
        float angle = 0.0;
        Vector2 sizeProportion = Vector2(1,1);

        Vector2 GenerateAndReturnAnchor()
        {
            this->GenerateAnchor();
            return this->anchor;
        }

        //
        // Criação de pontos
        //
        void AddSelectionPoint(int x, int y, int index)
        {
            this->corners[index] = Vector2(x, y);
        }
        void GenerateOriginPoints(void)
        {
            this->originPoints = new Vector2[this->elementsCounter];

            for (int i = 0; i < elementsCounter; i++)
            {
                originPoints[i] = this->points[i] - GenerateAndReturnAnchor();
            }
            for (int i = 0; i < 4; i++)
            {
                originCorners[i] = this->corners[i] - GenerateAndReturnAnchor();
            }
        }
        void AddPoint(int x, int y, int index)
        {
            this->points[index] = Vector2(x, y);
        }
        virtual void GenerateAnchor(void) {}

        bool shouldBeFilled = false;

        float r = 0;
        float g = 0;
        float b = 0;
};

#endif // DRAWING_H
