#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

class Color
{
public:
    float r, g, b, alpha;

    Color()
    {
        r = g = b = 0.5;
        alpha = 1.0;
    }

    Color(float r, float g, float b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->alpha = 1.0;
    }

    Color(float r, float g, float b, float alpha)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->alpha = alpha;
    }

    void set(float r, float g, float b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    void set(float r, float g, float b, float alpha)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->alpha = alpha;
    }

    void set(Color c)
    {
        this->r = c.r;
        this->g = c.g;
        this->b = c.b;
        this->alpha = c.alpha;
    }
};


#endif // COLOR_H_INCLUDED
