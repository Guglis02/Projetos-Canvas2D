#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <vector>
#include "Button.h"
#include "FunctionType.h"

class ToolBar
{
    public:
        ToolBar(int toolBarHeight, int toolBarWidth);

        Button* selectedButton = NULL;

        void Update(int toolBarHeight, int toolBarWidth);
        void CheckButtonCollision(int mx, int my);
        void CreateButton(int height, int width, FunctionType functionType, char* name, int* rgb);

        bool SelectedButtonExists();
        void DeSelectButton();

        FunctionType GetCurrentFunction();

    private:
        int toolBarHeight = 0;
        int toolBarWidth = 0;

        vector<Button*> buttons;
};

#endif // TOOLBAR_H
