#ifndef DRAWING_H
#define DRAWING_H

#include "../FunctionType.h"
#include "../PointsUtils.h"
#include "../Vector2.h"
#include "../gl_canvas2d.h"
#include "../Color.h"
#include <stdio.h>
#include <algorithm>
#include <iostream>

using namespace std;

/** \brief
Classe base dos desenhos, cont�m boa parte da l�gica dos mesmos.
 */
class Drawing
{
public:
    //
    // Render
    //
    void Render(void)
    {
        CV::color(colorRGB.r, colorRGB.g, colorRGB.b, colorRGB.alpha);
        float* xs = Vector2::GetXs(this->points, elementsCounter);
        float* ys = Vector2::GetYs(this->points, elementsCounter);
        if (shouldBeFilled)
        {
            CV::polygonFill(xs, ys, this->elementsCounter);
        }
        else
        {
            CV::polygon(xs, ys, elementsCounter);
        }

        delete[] xs;
        delete[] ys;
    }
    // Desenha caixa de sele��o, e seus pontos de intera��o
    void RenderSelectionIndicators(void)
    {
        float* xs = Vector2::GetXs(this->corners, 4);
        float* ys = Vector2::GetYs(this->corners, 4);

        CV::color(1);
        CV::polygon(xs, ys, 4);

        delete[] xs;
        delete[] ys;

        for (int i = 0; i < 4; i++)
        {
            CV::circleFill(corners[i].x, corners[i].y, IndicatorBallRadius, 10);
        }

        CV::circleFill(rotationIndicator.x, rotationIndicator.y, IndicatorBallRadius, 10);
    }
    // M�todo virtual chamado pelas classes descendentes,
    // chamado enquanto o usu�rio arrasta o mouse no canvas de desenho.
    virtual void RenderPrototype(int clickX, int clickY, int currentX, int currentY) {};

    //
    // Mouse
    //
    // Checa se o mouse clicou dentro do desenho
    bool CheckMouseClick(int mx, int my)
    {
        return pnpoly(this->elementsCounter,
                      Vector2::GetXs(this->points, elementsCounter),
                      Vector2::GetYs(this->points, elementsCounter),
                      mx, my);
    }
    // Checa se o mouse clicou em um ponto de intera��o,
    // e ativa a flag correspondente no desenho.
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
    // Edi��o de desenho (alguns m�todos recebem input do mouse, outros recebem input do arquivo de figuras salvas)
    //

    // Recebe input do usu�rio e determina o comportamento do desenho baseado na flag ativada.
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

    // Move o desenho de acordo com um vetor incremental
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

    // Redimensiona o desenho
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

        // Usa a dist�ncia dos pontos e as dimens�es originais do desenho pra calcular a propor��o do redimensionamento
        Vector2 proportion = Vector2(
                                 distanceToOrigin.x / width * 1.0,
                                 distanceToOrigin.y / height * 1.0);

        // Calcula dist�ncia entre os novos pontos e os pontos originais
        Vector2 distanceMoved = Vector2(
                                    DistanceBetweenTwoPoints(newOriginCorner1, GetPerpendicularPoint(originCorners[1], originCorners[2], newOriginCorner1)),
                                    DistanceBetweenTwoPoints(newOriginCorner3, GetPerpendicularPoint(originCorners[2], originCorners[3], newOriginCorner3)));

        // Se a dist�ncia dos novos pontos de controle em rela��o a suas posi��es originais
        // for maior do que a dist�ncia deles a origem, ou maior do que sua largura, implica que
        // o desenho foi invertido, logo, a propor��o � invertida e o desenho � espelhado
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

    // Recebe um vetor propor��o do arquivo de desenhos salvos e aplica
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

    // Recebe a posi��o do mouse e calcula o angulo que o desenho deve ser rotacionado
    // chama o m�todo de rota��o.
    void Rotate(int mx, int my)
    {
        Vector2 originToCenter = Vector2((corners[0] + corners[2]) / 2.0);

        float angle = GetAngleBetweenPoints(rotationIndicator - originToCenter,
                                            Vector2(mx, my) - originToCenter);

        ApplyAngle(angle);
    }

    // Recebe um �ngulo e aplica a rota��o correspondente no desenho
    void ApplyAngle(float angle)
    {
        Vector2 originToCenter = Vector2((corners[0] + corners[2]) / 2.0);

        // Rotaciona o indicador de rota��o na origem

        Vector2 originRotationPoint = rotationIndicator - originToCenter;

        Vector2 rotated = RotatePoint(originRotationPoint, angle);

        rotationIndicator = rotated + originToCenter;

        // Rotaciona os pontos do desenho na origem
        for (int i = 0; i < elementsCounter; i++)
        {
            rotated = RotatePoint(points[i] - originToCenter, angle);
            points[i] = rotated + originToCenter;
        }
        // Rotaciona os indicadores de sele��o na origem
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
        // Variaveis auxiliares para pegar os pontos extremos e montar o indicador de sele��o
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
    void SetColor(Color rgb)
    {
        this->colorRGB = rgb;
    }
    void SetFillFlag(bool value)
    {
        this->shouldBeFilled = value;
    }
    void SwitchFillable(void)
    {
        this->shouldBeFilled = !this->shouldBeFilled;
    }
    //
    // Getters
    //
    FunctionType GetType(void)
    {
        return this->type;
    }
    Color GetColor(void)
    {
        Color rgb = Color(this->colorRGB.r, this->colorRGB.g, this->colorRGB.b);
        return rgb;
    }
    bool GetFillFlag(void)
    {
        return this->shouldBeFilled;
    }
    float GetAngle(void)
    {
        return this->angle;
    }
    Vector2 GetProportion(void)
    {
        return this->sizeProportion;
    }
    Vector2* GetOriginPoints(void)
    {
        return this->originPoints;
    }
    float GetHeight(void)
    {
        return this->height;
    }
    float GetWidth(void)
    {
        return this->width;
    }
    Vector2 GetAnchor(void)
    {
        return this->anchor;
    }
    int GetElementsCount(void)
    {
        return elementsCounter;
    }
    Vector2 GetGlobalCenter(void)
    {
        return (this->corners[0] + (this->corners[2] - this->corners[0]) * 0.5);
    }

    bool isMoving = false;

protected:
    FunctionType type;
    int elementsCounter;

    Vector2* points;

    // Atributos que ser�o usados para restaurar o desenho ap�s o load
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

    // Atributos relacionados a modifica��o do desenho
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
    // Cria��o de pontos
    //
    void AddSelectionPoint(int x, int y, int index)
    {
        this->corners[index] = Vector2(x, y);
    }
    void GenerateOriginPoints(void)
    {
        delete[] this->originPoints;
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

    Color colorRGB;
private:
    // Posiciona o ponto de intera��o da rota��o
    void SetRotationPoint(void)
    {
        Vector2 selectionBoxTop = (this->corners[0] + this->corners[1]) * 0.5;
        float modifier = sizeProportion.y < 0 ? 1 : -1;

        this->rotationIndicator.x = selectionBoxTop.x + sin(angle) * modifier * -rotationIndicatorOffset;
        this->rotationIndicator.y = selectionBoxTop.y + cos(angle) * modifier * rotationIndicatorOffset;
    }
};

#endif // DRAWING_H
