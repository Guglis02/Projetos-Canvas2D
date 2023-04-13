#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <vector>
#include "Button.h"
#include "FunctionType.h"

using namespace std;

class ToolBar
{
    public:
        ToolBar(int height, int width)
        {
            this->height = height;
            this->width = width;
        }

        void Update(int x, int y, int height, int width)
        {
            color(1);
            rectFill(x, y, x + width, height);

            int buttonsCount = buttons.size();
            int halfRoundedCount = (buttonsCount + 1)/2;

            for (int i = 0; i < buttonsCount; i++)
            {
                if (i < halfRoundedCount)
                {
                    buttons[i]->Update(x + width/halfRoundedCount * (i%buttonsCount), 10);
                } else
                {
                    buttons[i]->Update(x + width/halfRoundedCount * (i%halfRoundedCount), 70);
                }
            }
        }

        void CreateButton(int height, int width, FunctionType functionType, function<void()> callback, char* name, float* rgb)
        {
            Button* newButton = new Button(height,
                                           width,
                                           functionType,
                                           callback,
                                           name);
            newButton->SetColor(rgb[0], rgb[1], rgb[2]);
            buttons.push_back(newButton);
        }

        void CreateButton(FunctionType functionType, function<void()> callback, char* name)
        {
            Button* newButton = new Button(defaultButtonHeight,
                                           defaultButtonWidth,
                                           functionType,
                                           callback,
                                           name);
            newButton->SetColor(defaultButtonColor[0],
                                defaultButtonColor[1],
                                defaultButtonColor[2]);
            buttons.push_back(newButton);
        }

        bool CheckButtonCollision(int mx, int my)
        {
            for (Button* b : buttons)
            {
                if (b->CheckMouseClick(mx, my))
                {
                    if(b->GetSelectedState())
                    {
                        DeSelectButton();
                        selectedButton = b;
                        selectedButton->SetSelectedState(true);
                    }
                    return true;
                }
            }
            return false;
        }

        void DeSelectButton(void)
        {
            if (!selectedButton)
            {
                return;
            }

            selectedButton->SetSelectedState(false);
            selectedButton = NULL;
        }

        Button* GetSelectedButton(void)
        {
            return this->selectedButton;
        }
        FunctionType GetCurrentFunction(void)
        {
            if (selectedButton)
            {
                return selectedButton->GetFunction();
            }

            return None;
        }
    private:
        int height = 0;
        int width = 0;

        const int defaultButtonWidth = 80;
        const int defaultButtonHeight = 50;
        const float defaultButtonColor[3] = {0, 0.5, 1};

        vector<Button*> buttons;
        Button* selectedButton = NULL;
};

#endif // TOOLBAR_H
