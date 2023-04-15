#ifndef COLORUTILS_H_INCLUDED
#define COLORUTILS_H_INCLUDED

static void HSVtoRGB (float h, float s, float v, float& r, float& g, float& b)
{
    if (s == 0.0f)
    {
        // Se a saturação for 0, a cor vai ser em tons de cinza
        r = v;
        g = v;
        b = v;
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
                r = v;
                g = t;
                b = p;
                break;
            case 1:
                r = q;
                g = v;
                b = p;
                break;
            case 2:
                r = p;
                g = v;
                b = t;
                break;
            case 3:
                r = p;
                g = q;
                b = v;
                break;
            case 4:
                r = t;
                g = p;
                b = v;
                break;
            default:
                r = v;
                g = p;
                b = q;
                break;
        }
    }
}

static void RGBtoHSV(float r, float g, float b, float& h, float& s, float& v)
{
    float minVal = min(min(r, g), b);
    float maxVal = max(max(r, g), b);
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
        if (r == maxVal)
        {
            h = (g - b) / delta;
        }
        else if (g == maxVal)
        {
            h = 2.0f + (b - r) / delta;
        }
        else
        {
            h = 4.0f + (r - g) / delta;
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
