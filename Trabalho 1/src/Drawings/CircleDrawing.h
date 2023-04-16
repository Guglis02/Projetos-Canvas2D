#ifndef CIRCLEDRAWING_H_INCLUDED
#define CIRCLEDRAWING_H_INCLUDED

#include "Drawing.h"
#include "PointsUtils.h"

/** \brief
Classe responsável pelo desenho em formato de círculo.
 */
class CircleDrawing : public Drawing
{
public:
    CircleDrawing(int x, int y, int radius, int div)
    {
        this->type = Circle;

        this->points = new Vector2[div];

        this->elementsCounter = div;

        this->ang = 0;
        this->inc = PI_2/div;

        for(int i = 0; i < div; i++)
        {
            this->AddPoint(x + (cos(ang)*radius), y + (sin(ang)*radius), i);
            ang+=inc;
        }

        SetSelectionPoints();
        GenerateOriginPoints();
        GenerateAnchor();
    }

    void RenderPrototype(int clickX, int clickY, int currentX, int currentY)
    {
        this->ang = 0;
        this->inc = PI_2/this->elementsCounter;
        this->radius = DistanceBetweenTwoPoints(clickX, clickY, currentX, currentY);

        for(int i = 0; i < this->elementsCounter; i++)
        {
            this->AddPoint(clickX + (cos(ang)*radius),
                           clickY + (sin(ang)*radius), i);
            ang+=inc;
        }

        SetSelectionPoints();
        GenerateOriginPoints();
        GenerateAnchor();
        Render();
    }

    void GenerateAnchor(void)
    {
        this->anchor = GetGlobalCenter();
    }
private:
    float ang = 0;
    float inc = 0;
    float radius = 0;
};

#endif // CIRCLEDRAWING_H_INCLUDED
