#ifndef SLIDER_H_INCLUDED
#define SLIDER_H_INCLUDED

#include "../gl_canvas2d.h"
#include <math.h>

using namespace std;

/** \brief
Classe que representa um slider.
*/
class Slider : public Interactable
{
public:
    Slider(float width, float height, float minVal, float maxVal, float initialValue, const string& name)
        : Interactable(height, width, name)
    {
        this->minVal = minVal;
        this->maxVal = maxVal;
        this->currentValue = initialValue;
        this->handlerWidth = width * 0.03;
        this->isDragging = false;
    }

    // Desenha a barra
    void Render(void)
    {
        CV::color(0.5f, 0.5f, 0.5f);
        CV::rectFill(left, bottom, right, top);

        RenderHandler();
        RenderLabels();
    }

    void RenderLabels(void)
    {
        char numericLabel[64];
        CV::color(0);

        sprintf(numericLabel, "%.1f", minVal);
        CV::text(left, bottom - 10, numericLabel);
        sprintf(numericLabel, "%.1f", maxVal);
        CV::text(right, bottom - 10, numericLabel);

        CV::text(left, top + 10, "Nome");
    }

    // Desenha o "pegador" da barra
    void RenderHandler(void)
    {
        CV::color(1.0, 1.0, 1.0);
        CV::rectFill( HandlerPos() - handlerWidth, bottom,
                      HandlerPos() + handlerWidth, top);
    }

    float HandlerPos(void)
    {
        return left + currentValue * width;
    }

    void OnMouseClick(float mx, float my)
    {
        if (IsMouseInside(mx, my))
        {
            isDragging = true;
        }
    }

    void OnMouseDrag(float mx)
    {
        if (isDragging)
        {
            currentValue = (mx - left) / width;
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

protected:
    float minVal;
    float maxVal;
    float currentValue;
    float handlerWidth;
    bool isDragging = false;
};

#endif // SLIDER_H_INCLUDED
