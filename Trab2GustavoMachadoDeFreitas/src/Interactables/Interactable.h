#ifndef INTERACTABLE_H_INCLUDED
#define INTERACTABLE_H_INCLUDED

#include "../gl_canvas2d.h"
#include <string>

using namespace std;

class Interactable
{
public:
    Interactable(int height, int width, const string& name)
    {
        this->height = height;
        this->width = width;
        this->name = name;
    }

    void Update(int x, int y)
    {
        this->top = y + height;
        this->bottom = y;
        this->left = x;
        this->right = x + width;
    }

    virtual void Render(void){}

    bool IsMouseInside(int mx, int my)
    {
        return my <= top && my >= bottom && mx >= left && mx <= right;
    }
protected:
    int height;
    int width;

    int top;
    int bottom;
    int left;
    int right;

    string name;
};

#endif // INTERACTABLE_H_INCLUDED
