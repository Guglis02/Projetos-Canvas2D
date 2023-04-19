#ifndef BUTTON_H
#define BUTTON_H

#include "../FunctionType.h"
#include "../gl_canvas2d.h"
#include <string>
#include <functional>

using namespace std;

/** \brief
Classe que representa um bot�o.
*/
class Button
{
public:
    Button(int height, int width, FunctionType functionType, function<void()> callback, char* name)
    {
        this->height = height;
        this->width = width;
        this->functionType = functionType;
        this->callback = callback;
        this->name = name;
    }

    ~Button()
    {
        delete[] name;
    }

    void Update(int x, int y)
    {
        this->top = y;
        this->bottom = y + height;
        this->left = x;
        this->right = x + width;

        if (selectedState)
        {
            CV::color(0, 1, 0);
        }
        else
        {
            CV::color(r, g, b);
        }

        CV::rectFill(left, top, right, bottom);
        CV::color(0);
        CV::text(x, y + 10, name);
    }

    // Checa o clique do mouse
    // Se o bot�o possui uma callback, chama ela
    // Sen�o, trata-se de um bot�o selecion�vel, ent�o a flag � atualizada
    bool CheckMouseClick(int mx, int my)
    {
        if (my >= top && my <= bottom && mx >= left && mx <= right)
        {
            if (callback)
            {
                callback();
            }
            else
            {
                this->selectedState = !this->selectedState;
            }
            return true;
        }
        return false;
    }

    bool GetSelectedState(void)
    {
        return this->selectedState;
    }
    void SetSelectedState(bool state)
    {
        this->selectedState = state;
    }

    void SetColor(float r, float g, float b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    float r = 1;
    float g = 1;
    float b = 1;

    FunctionType GetFunction(void)
    {
        return this->functionType;
    }
private:
    int height;
    int width;

    int top;
    int bottom;
    int left;
    int right;

    FunctionType functionType;
    function<void()> callback;

    char* name;

    bool selectedState = false;
};

#endif // BUTTON_H
