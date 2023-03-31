#ifndef BUTTON_H
#define BUTTON_H

#include "FunctionType.h"
#include <string>

using namespace std;

class Button
{
    public:
        Button(int height, int width, FunctionType functionType, char* name);

        void Update(int x, int y);
        bool CheckMouseClick(int mx, int my);

        bool GetSelectedState();
        void SetSelectedState(bool state);

        void SetColor(float r, float g, float b);
        float r = 1;
        float g = 1;
        float b = 1;

        FunctionType GetFunction();
    private:
        int height;
        int width;

        int top;
        int bottom;
        int left;
        int right;

        FunctionType functionType;

        char* name;

        bool selectedState = false;
};

#endif // BUTTON_H
