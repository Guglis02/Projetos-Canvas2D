#ifndef BAR_H_INCLUDED
#define BAR_H_INCLUDED

#include "Color.h"

/** \brief
Classe base das barras da interface.
Possui um método de atualização e um de checagem do mouse.
 */
class Bar
{
public:
    Bar(int height, int width)
    {
        this->height = height;
        this->width = width;
        this->backgroundColor = Color(0.5, 0.5, 0.5);
    }

    void Update(int x, int y, int height, int width)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        color(backgroundColor.r, backgroundColor.g, backgroundColor.b);
        rectFill(x, y, x + width, height);
    }

    bool IsMouseInside(int mx, int my)
    {
        return my >= y && my <= y + height && mx >= x && mx <= x + width;
    }
protected:
    int x = 0;
    int y = 0;
    int height = 0;
    int width = 0;
    Color backgroundColor;
};

#endif // BAR_H_INCLUDED
