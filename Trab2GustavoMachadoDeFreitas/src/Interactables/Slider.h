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
    Slider(float width, float height, float minVal, float maxVal, float initialValue, const string& name, function<void(float)> callback)
        : Interactable(width, height, name)
    {
        this->minVal = minVal;
        this->maxVal = maxVal;
        this->callback = callback;
        this->SetValue(initialValue);
        this->handlerWidth = width * 0.03;
        this->isDragging = false;
    }

    // Desenha a barra
    void Render(void)
    {
        CV::color(0);
        CV::rectFill(left, bottom, left + width, bottom + height);

        RenderHandler();
        RenderLabels();
    }

    void RenderLabels(void)
    {
        char numericLabel[64];
        CV::color(0);

        sprintf(numericLabel, "%.0f", minVal);
        CV::text(left, top + padding, numericLabel);
        sprintf(numericLabel, "%.0f", maxVal);
        CV::text(right - padding * 2, top + padding, numericLabel);

        CV::text(left, top + padding * 2, name.c_str());
    }

    // Desenha o "pegador" da barra
    void RenderHandler(void)
    {
        CV::color(13);
        CV::rectFill( HandlerPos() - handlerWidth, bottom,
                      HandlerPos() + handlerWidth, top);
    }

    float HandlerPos(void)
    {
        return left + currentValue * width;
    }

    void OnClick()
    {
        isDragging = true;
    }

    void OnDrag(float mx)
    {
        if (isDragging)
        {
            currentValue = (mx - left) / width;
            currentValue = min(max(currentValue, 0.0f), 1.0f);
            OnValueChanged();
        }
    }

    void OnRelease(void)
    {
        isDragging = false;
    }

    void OnValueChanged(void)
    {
        float newValue = GetValue();
        callback(newValue);
    }

    float GetValue(void)
    {
        return currentValue * (maxVal - minVal) + minVal;
    }

    void SetValue(float value)
    {
        this->currentValue = (value - minVal) / (maxVal - minVal);
        OnValueChanged();
    }

protected:
    const int padding = 11;
    function<void(float)> callback;

    float minVal;
    float maxVal;
    float currentValue;

    float handlerWidth;

    bool isDragging = false;
};

#endif // SLIDER_H_INCLUDED
