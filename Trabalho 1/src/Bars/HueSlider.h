#ifndef HUESLIDER_H_INCLUDED
#define HUESLIDER_H_INCLUDED

#include "ColorUtils.h"

class HueSlider : public Slider
{
public:
    HueSlider(float x, float y, float width, float height, float minVal, float maxVal, float initialValue)
    : Slider(x, y, width, height, minVal, maxVal, initialValue)
    {}

    void Render(float sat, float value) {
        float hueRange = maxVal - minVal;
        float hueStep = hueRange / width;
        float r, g, b;
        for (int i = 0; i < width; i++) {
            float hue = minVal + i * hueStep;
            HSVtoRGB(hue, sat, value, r, g, b);
            color(r, g, b);
            rectFill(x + i, y, x + i + 1, y + height);
        }

        RenderHandler();
    }
};

#endif // HUESLIDER_H_INCLUDED
