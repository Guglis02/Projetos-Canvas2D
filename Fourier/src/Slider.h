#ifndef SLIDER_H_INCLUDED
#define SLIDER_H_INCLUDED

#include "./gl_canvas2d.h"
#include <math.h>

using namespace std;

/** \brief
Classe que representa um slider.
*/
class Slider
{
public:
    Slider(float x, float y, float width, float height, float minVal, float maxVal, float initialValue)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->minVal = minVal;
        this->maxVal = maxVal;
        this->currentValue = initialValue;
        this->handlerWidth = width * 0.03;
        this->isDragging = false;
    }

    void Update(float x, float y, float width, float height)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

    // Desenha a barra
    void Render(void)
    {
        CV::color(0.5f, 0.5f, 0.5f);
        CV::rectFill(x, y, x + width, y + height);

        RenderHandler();
    }

    // Desenha o "pegador" da barra
    void RenderHandler(void)
    {
        CV::color(1.0, 1.0, 1.0);
        CV::rectFill(x + currentValue * width - handlerWidth, y,
                 x + currentValue * width + handlerWidth, y + height);
    }

    void OnMouseClick(float mx, float my)
    {
        if (mx >= x + currentValue * width - handlerWidth
                && mx <= x + currentValue * width + handlerWidth
                && my >= y && my <= y + height)
        {
            isDragging = true;
        }
    }

    void OnMouseDrag(float mx)
    {
        if (isDragging)
        {
            currentValue = (mx - x) / width;
            currentValue = min(max(currentValue, minVal), maxVal);
        }
    }

    void OnMouseRelease(void)
    {
        isDragging = false;
    }

    float GetValue(void)
    {
        return currentValue * (maxVal - minVal) + minVal;
    }

    void SetValue(float value)
    {
        this->currentValue = (value - minVal) / (maxVal - minVal);
    }

    bool IsMouseInside(int mx, int my)
    {
        return my >= y && my <= y + height && mx >= x && mx <= x + width;
    }

protected:
    float x;
    float y;
    float width;
    float height;
    float minVal;
    float maxVal;
    float currentValue;
    float handlerWidth;
    bool isDragging = false;
};

#endif // SLIDER_H_INCLUDED
