#ifndef VALUESLIDER_H_INCLUDED
#define VALUESLIDER_H_INCLUDED

#include "ColorUtils.h"

class ValueSlider : public Slider
{
public:
    ValueSlider(float x, float y, float width, float height, float minVal, float maxVal, float initialValue)
    : Slider(x, y, width, height, minVal, maxVal, initialValue)
    {}

    void Render(float hue, float sat) {
        for (int i = 0; i < width; i++) {
            float v = static_cast<float>(i) / width;
            float r, g, b;
            HSVtoRGB(hue, sat, v, r, g, b);

            color(r, g, b);
            rectFill(x + i, y, x + i + 1, y + height);
        }

        RenderHandler();
    }
};

#endif // VALUESLIDER_H_INCLUDED
