#ifndef BUTTON_H
#define BUTTON_H

#include "../gl_canvas2d.h"
#include "Interactable.h"
#include <string>
#include <functional>

using namespace std;

/** \brief
Classe que representa um botão.
*/
class Button : public Interactable
{
public:
    Button(int height, int width, function<void()> callback, const string& name)
        : Interactable(height, width, name)
    {
        this->callback = callback;
    }

    void SetDesc(const string& desc)
    {
        this->desc = desc;
    }

    void Render(void)
    {
        CV::color(r, g, b);
        CV::rectFill(left, top, right, bottom);
        CV::color(0);
        CV::text(left + 5, bottom + 30, name.c_str());
        if (!desc.empty())
        {
            CV::text(left + 5, bottom + 10, desc.c_str());
        }
    }

    void CheckMouseClick(int mx, int my)
    {
        if (IsMouseInside(mx, my))
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
    function<void()> callback;

    float r = 1;
    float g = 1;
    float b = 1;

    string desc;
};

#endif // BUTTON_H
