#ifndef SLIDER_H_INCLUDED
#define SLIDER_H_INCLUDED

#include "gl_canvas2d.h"
#include <math.h>

using namespace std;

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
        this->sliderWidth = width * 0.1;
        this->isDragging = false;
    }

    void Update(float x, float y, float width, float height)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

    void Render(void)
    {
        // Desenha a barra
        color(0.5f, 0.5f, 0.5f);
        rectFill(x, y, x + width, y + height);

        RenderHandler();
    }

    void RenderHandler(void)
    {
        // Desenha o handler
        color(1.0f, 1.0f, 1.0f);
        rectFill(x + currentValue * width - sliderWidth * 0.5, y,
                 x + currentValue * width + sliderWidth * 0.5, y + height);
    }

    void OnMouseClick(float mx, float my)
    {
        if (mx >= x + currentValue * width - sliderWidth * 0.5
                && mx <= x + currentValue * width + sliderWidth * 0.5
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

    float SetValue(float value)
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
    float sliderWidth;
    bool isDragging = false;
};

#endif // SLIDER_H_INCLUDED
