#include "Button.h"
#include "gl_canvas2d.h"
#include "FunctionType.h"

Button::Button(int height, int width, FunctionType functionType, char* name)
{
    this->height = height;
    this->width = width;
    this->functionType = functionType;
    this->name = name;

    this->isSelectable = true;
}

void Button::Update(int x, int y)
{
    this->top = (int)(y - height * 0.5);
    this->bottom = (int)(y + height * 0.5);
    this->left = (int)(x - width * 0.5);
    this->right = (int)(x + width * 0.5);

    if (selectedState)
    {
        color(3);
    }
    else
    {
        color(4);
    }

    translate(left, top);
    rectFill(0, 0, width, height);
    color(0);
    text(0, 10, name);
    translate(0,0);
}

void Button::CheckMouseClick(int mx, int my)
{
    if (my >= top && my <= bottom && mx >= left && mx <= right)
    {
        printf("\n%d", selectedState);
        this->selectedState = !selectedState;
    }
}

bool Button::GetSelectedState()
{
    return this->selectedState;
}

void Button::SetSelectedState(bool state)
{
    this->selectedState = state;
}

FunctionType Button::GetFunction()
{
    return this->functionType;
}

