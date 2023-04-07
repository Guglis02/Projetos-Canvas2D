#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <vector>
#include "Button.h"
#include "FunctionType.h"

class ToolBar
{
    public:
        ToolBar(int height, int width);

        Button* selectedButton = NULL;

        void Update(int x, int y, int height, int width);
        bool CheckButtonCollision(int mx, int my);
        void CreateButton(int height, int width, FunctionType functionType, function<void()> callback, char* name, float* rgb);
        void CreateButton(FunctionType functionType, function<void()> callback, char* name);

        bool SelectedButtonExists();
        void DeSelectButton();

        FunctionType GetCurrentFunction();

    private:
        int height = 0;
        int width = 0;

        vector<Button*> buttons;
};

#endif // TOOLBAR_H
