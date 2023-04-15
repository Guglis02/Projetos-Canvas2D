#ifndef SATSLIDER_H_INCLUDED
#define SATSLIDER_H_INCLUDED

#include "ColorUtils.h"

class SatSlider : public Slider
{
public:
    SatSlider(float x, float y, float width, float height, float minVal, float maxVal, float initialValue)
    : Slider(x, y, width, height, minVal, maxVal, initialValue)
    {}

    void Render(float hue, float value) {
        float satRange = maxVal - minVal;
        float satStep = satRange / width;
        float r, g, b;
        for (int i = 0; i < width; i++) {
            float sat = minVal + i * satStep;
            HSVtoRGB(hue, sat, value, r, g, b);
            color(r, g, b);
            rectFill(x + i, y, x + i + 1, y + height);
        }
        RenderHandler();
    }
};

#endif // SATSLIDER_H_INCLUDED
