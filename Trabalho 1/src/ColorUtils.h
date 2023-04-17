#ifndef COLORUTILS_H_INCLUDED
#define COLORUTILS_H_INCLUDED

#include "Color.h"

/** \brief
 * Arquivo utilitário contendo funções relacionadas a conversão de cores.
 */

static Color HSVtoRGB (float h, float s, float v)
{
    Color rgb;
    if (s == 0.0f)
    {
        // Se a saturação for 0, a cor vai ser em tons de cinza
        rgb.r = v;
        rgb.g = v;
        rgb.b = v;
    }
    else
    {
        h = fmodf(h, 360.0f) / 60.0f; // Converte o hue para o intervalo [0, 6)
        int i = static_cast<int>(h);
        float f = h - i; // Parte fracionária do hue

        float p = v * (1.0f - s);
        float q = v * (1.0f - s * f);
        float t = v * (1.0f - s * (1.0f - f));

        switch (i)
        {
        case 0:
            rgb.r = v;
            rgb.g = t;
            rgb.b = p;
            break;
        case 1:
            rgb.r = q;
            rgb.g = v;
            rgb.b = p;
            break;
        case 2:
            rgb.r = p;
            rgb.g = v;
            rgb.b = t;
            break;
        case 3:
            rgb.r = p;
            rgb.g = q;
            rgb.b = v;
            break;
        case 4:
            rgb.r = t;
            rgb.g = p;
            rgb.b = v;
            break;
        default:
            rgb.r = v;
            rgb.g = p;
            rgb.b = q;
            break;
        }
    }

    return rgb;
}

static void RGBtoHSV(Color rgb, float& h, float& s, float& v)
{
    float minVal = min(min(rgb.r, rgb.g), rgb.b);
    float maxVal = max(max(rgb.r, rgb.g), rgb.b);
    float delta = maxVal - minVal;

    if (maxVal == 0.0f)
    {
        s = 0.0f;
    }
    else
    {
        s = delta / maxVal;
    }

    if (delta == 0.0f)
    {
        h = 0.0f;
    }
    else
    {
        if (rgb.r == maxVal)
        {
            h = (rgb.g - rgb.b) / delta;
        }
        else if (rgb.g == maxVal)
        {
            h = 2.0f + (rgb.b - rgb.r) / delta;
        }
        else
        {
            h = 4.0f + (rgb.r - rgb.g) / delta;
        }

        h *= 60.0f;

        if (h < 0.0f)
        {
            h += 360.0f;
        }
    }
    v = maxVal;
}

#endif // COLORUTILS_H_INCLUDED
