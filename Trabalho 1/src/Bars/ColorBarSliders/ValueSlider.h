#ifndef VALUESLIDER_H_INCLUDED
#define VALUESLIDER_H_INCLUDED

#include "../../ColorUtils.h"

/** \brief
Slider de value, muda de apar�ncia de acordo com o valor das outras barras
 */
class ValueSlider : public Slider
{
public:
    ValueSlider(float x, float y, float width, float height, float minVal, float maxVal, float initialValue)
        : Slider(x, y, width, height, minVal, maxVal, initialValue)
    {}

    void Render(float hue, float sat)
    {
        for (int i = 0; i < width; i++)
        {
            float value = static_cast<float>(i) / width;
            Color rgb = HSVtoRGB(hue, sat, value);
            CV::color(rgb.r, rgb.g, rgb.b);
            CV::rectFill(x + i, y, x + i + 1, y + height);
        }

        RenderHandler();
    }
};

#endif // VALUESLIDER_H_INCLUDED
