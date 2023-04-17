#ifndef HUESLIDER_H_INCLUDED
#define HUESLIDER_H_INCLUDED

#include "ColorUtils.h"

/** \brief
Slider de Hue, muda de aparência de acordo com o valor das outras barras
 */
class HueSlider : public Slider
{
public:
    HueSlider(float x, float y, float width, float height, float minVal, float maxVal, float initialValue)
        : Slider(x, y, width, height, minVal, maxVal, initialValue)
    {}

    void Render(float sat, float value)
    {
        float hueRange = maxVal - minVal;
        float hueStep = hueRange / width;
        for (int i = 0; i < width; i++)
        {
            float hue = minVal + i * hueStep;
            Color rgb = HSVtoRGB(hue, sat, value);
            color(rgb.r, rgb.g, rgb.b);
            rectFill(x + i, y, x + i + 1, y + height);
        }

        RenderHandler();
    }
};

#endif // HUESLIDER_H_INCLUDED
