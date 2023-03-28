#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <vector>
#include "Button.h"

class ToolBar
{
    public:
        ToolBar(int toolBarHeight, int toolBarWidth);

        Button* selectedButton = NULL;

        void Update(int toolBarHeight, int toolBarWidth);
        void CheckButtonCollision(int mx, int my);

    private:
        int toolBarHeight = 0;
        int toolBarWidth = 0;
        vector<Button*> buttons;

        void StartButtons();
};

#endif // TOOLBAR_H
