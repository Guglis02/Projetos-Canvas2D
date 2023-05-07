#ifndef BUTTONBAR_H_INCLUDED
#define BUTTONBAR_H_INCLUDED

#include <functional>
#include "Interactables/Button.h"
#include "Interactables/Slider.h"

using namespace std;

class ButtonBar
{
public:
    ButtonBar(int height, int width)
    {
        this->height = height;
        this->width = width;
    }

    void Update(int height, int width)
    {
        this->width = width;
        this->height = height;
        CV::color(1);
        CV::rectFill(x, y, x + width, height);

        int buttonsCount = buttons.size();
        int halfRoundedCount = (buttonsCount + 1) >> 1;

        for (int i = 0; i < buttonsCount; i++)
        {
            if (i < halfRoundedCount)
            {
                buttons[i]->Update(x + width/halfRoundedCount * (i%buttonsCount), 60);
            }
            else
            {
                buttons[i]->Update(x + width/halfRoundedCount * (i%halfRoundedCount), 5);
            }
            buttons[i]->Render();
        }
    }

    void CreateButton(const char* name, function<void()> callback)
    {
        Button* newButton = new Button(defaultButtonHeight,
                                       defaultButtonWidth,
                                       callback,
                                       name);
        newButton->SetColor(defaultButtonColor[0],
                            defaultButtonColor[1],
                            defaultButtonColor[2]);
        buttons.push_back(newButton);
    }

    void CreateButton(const char* name, const char* desc, function<void()> callback)
    {
        Button* newButton = new Button(defaultButtonHeight,
                                       defaultButtonWidth,
                                       callback,
                                       name);
        newButton->SetColor(defaultButtonColor[0],
                            defaultButtonColor[1],
                            defaultButtonColor[2]);
        newButton->SetDesc(desc);
        buttons.push_back(newButton);
    }

    void CheckMouseClick(int mx, int my)
    {
        for (Button* b : buttons)
        {
            b->CheckMouseClick(mx, my);
        }
    }

private:
    int x = 0;
    int y = 0;
    int height = 0;
    int width = 0;

    const int defaultButtonWidth = 100;
    const int defaultButtonHeight = 50;
    const float defaultButtonColor[3] = {0, 0.5, 1};

    vector<Button*> buttons;
};


#endif // BUTTONBAR_H_INCLUDED
