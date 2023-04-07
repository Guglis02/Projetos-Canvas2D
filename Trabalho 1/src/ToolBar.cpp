#include "ToolBar.h"
#include "gl_canvas2d.h"

ToolBar::ToolBar(int height, int width)
{
    this->height = height;
    this->width = width;
}

void ToolBar::Update(int x, int y, int height, int width)
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

void ToolBar::CreateButton(int height, int width, FunctionType functionType, function<void()> callback, char* name, float* rgb)
{
    Button* newButton = new Button(height,
                                   width,
                                   functionType,
                                   callback,
                                   name);
    newButton->SetColor(rgb[0], rgb[1], rgb[2]);
    buttons.push_back(newButton);
}

int defaultButtonWidth = 80;
int defaultButtonHeight = 50;
float defaultButtonColor[] = {0, 0.5, 1};

void ToolBar::CreateButton(FunctionType functionType, function<void()> callback, char* name)
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

bool ToolBar::CheckButtonCollision(int mx, int my)
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

bool ToolBar::SelectedButtonExists()
{
    return selectedButton != NULL;
}

void ToolBar::DeSelectButton()
{
    if (!selectedButton)
    {
        return;
    }

    selectedButton->SetSelectedState(false);
    selectedButton = NULL;
}

FunctionType ToolBar::GetCurrentFunction()
{
    if (selectedButton)
    {
        return selectedButton->GetFunction();
    }

    return None;
}


