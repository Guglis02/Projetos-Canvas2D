#include "Button.h"
#include "gl_canvas2d.h"
#include "FunctionType.h"

Button::Button(int height, int width, FunctionType functionType, char* name)
{
    this->height = height;
    this->width = width;
    this->functionType = functionType;
    this->name = name;
}

void Button::Update(int x, int y)
{
    this->top = (int)(y);
    this->bottom = (int)(y + height);
    this->left = (int)(x);
    this->right = (int)(x + width);

    if (selectedState)
    {
        color(0, 1, 0);
    }
    else
    {
        color(r, g, b);
    }

    rectFill(left, top, right, bottom);
    color(0);
    text(x, y + 10, name);
}

bool Button::CheckMouseClick(int mx, int my)
{
    if (my >= top && my <= bottom && mx >= left && mx <= right)
    {
        this->selectedState = !this->selectedState;
        return true;
    }
    return false;
}

bool Button::GetSelectedState()
{
    return this->selectedState;
}

void Button::SetSelectedState(bool state)
{
    this->selectedState = state;
}

void Button::SetColor(float r, float g, float b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

FunctionType Button::GetFunction()
{
    return this->functionType;
}

