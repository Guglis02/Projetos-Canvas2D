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
    Button(int width, int height, function<void()> callback, const string& name, const string& desc)
        : Interactable(width, height, name)
    {
        this->callback = callback;
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

    void OnClick(void)
    {
        callback();
    }

    void SetColor(const float rgb[3])
    {
        this->r = rgb[0];
        this->g = rgb[1];
        this->b = rgb[2];
    }
private:
    function<void()> callback;

    float r = 1;
    float g = 1;
    float b = 1;

    string desc;
};

#endif // BUTTON_H
