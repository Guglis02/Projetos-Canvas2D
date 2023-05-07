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
            int interactableSpace = x + width/halfRoundedCount;
            if (i < halfRoundedCount)
            {
                buttons[i]->Update(leftBorder + interactableSpace * (i%buttonsCount), 60);
            }
            else
            {
                buttons[i]->Update(leftBorder + interactableSpace * (i%halfRoundedCount), 5);
            }
            buttons[i]->Render();
        }
    }

    void CreateButton(const char* name, const char* desc, function<void()> callback)
    {
        Button* newButton = new Button(defaultButtonWidth, defaultButtonHeight,
                                       callback, name, desc);
        newButton->SetColor(defaultButtonColor);
        buttons.push_back(newButton);
    }

    void CreateSlider(const char* name, float minVal, float maxVal, float initialValue, function<void(float)> callback)
    {
        Slider* newSlider = new Slider(defaultSliderWidth, defaultSliderHeight,
                                       minVal, maxVal, initialValue, name, callback);
        buttons.push_back(newSlider);
    }

    void CheckMouseClick(int mx, int my)
    {
        for (Interactable* b : buttons)
        {
            if (b->IsMouseInside(mx, my))
            {
                b->OnClick();
            }
        }
    }

    void OnMouseDrag(int mx)
    {
        for (Interactable* b : buttons)
        {
            b->OnDrag(mx);
        }
    }

    void OnMouseRelease(void)
    {
        for (Interactable* b : buttons)
        {
            b->OnRelease();
        }
    }

private:
    const int leftBorder = 10;
    int x = 0;
    int y = 0;
    int height = 0;
    int width = 0;

    const int defaultSliderWidth = 150;
    const int defaultSliderHeight = 20;
    const int defaultButtonWidth = 100;
    const int defaultButtonHeight = 50;
    const float defaultButtonColor[3] = {0, 0.5, 1};

    vector<Interactable*> buttons;
};


#endif // BUTTONBAR_H_INCLUDED
