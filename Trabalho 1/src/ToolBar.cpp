#include "ToolBar.h"
#include "gl_canvas2d.h"

ToolBar::ToolBar(int toolBarHeight, int toolBarWidth)
{
    this->toolBarHeight = toolBarHeight;
    this->toolBarWidth = toolBarWidth;
}

void ToolBar::Update(int toolBarHeight, int toolBarWidth)
{
    color(1);
    rectFill(0, 0, toolBarWidth, toolBarHeight);

    int buttonsCount = buttons.size();

    for (int i = 0; i < buttonsCount; i++)
    {
        buttons[i]->Update(toolBarWidth/buttonsCount * (i), 10);
    }
}

void ToolBar::CreateButton(int height, int width, FunctionType functionType, char* name, int* rgb)
{
    Button* newButton = new Button(height, width, functionType, name);
    newButton->SetColor(rgb[0], rgb[1], rgb[2]);
    buttons.push_back(newButton);
}

void ToolBar::CheckButtonCollision(int mx, int my)
{
    for (Button* b : buttons)
    {
        b->CheckMouseClick(mx, my);
        if (b->GetSelectedState())
        {
            if (selectedButton)
            {
                selectedButton->SetSelectedState(false);
            }
            selectedButton = b;
        }
    }
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


