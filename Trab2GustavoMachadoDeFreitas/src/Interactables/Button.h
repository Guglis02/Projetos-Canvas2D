#ifndef BUTTON_H
#define BUTTON_H

#include "../gl_canvas2d.h"
#include <string>
#include <functional>

using namespace std;

/** \brief
Classe que representa um botão.
*/
class Button
{
public:
    Button(int height, int width, function<void()> callback, const string& name)
    {
        this->height = height;
        this->width = width;
        this->callback = callback;
        this->name = name;
    }

    void SetDesc(const string& desc)
    {
        this->desc = desc;
    }

    void Update(int x, int y)
    {
        this->top = y;
        this->bottom = y + height;
        this->left = x;
        this->right = x + width;

        CV::color(r, g, b);
        CV::rectFill(left, top, right, bottom);
        CV::color(0);
        CV::text(x + 5, y + 30, name.c_str());
        if (!desc.empty())
        {
            CV::text(x + 5, y + 10, desc.c_str());
        }
    }

    void CheckMouseClick(int mx, int my)
    {
        if (my >= top && my <= bottom && mx >= left && mx <= right)
        {
            if (callback)
            {
                callback();
            }
        }
    }

    void SetColor(float r, float g, float b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
private:
    int height;
    int width;

    int top;
    int bottom;
    int left;
    int right;

    function<void()> callback;

    float r = 1;
    float g = 1;
    float b = 1;

    string name;
    string desc;
};

#endif // BUTTON_H
