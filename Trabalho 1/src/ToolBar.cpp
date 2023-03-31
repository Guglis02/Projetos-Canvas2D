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
        // Desenha botões de cores
        if (buttons[i]->GetFunction() == Color)
        {
            if (i < buttonsCount - 8)
            {
                buttons[i]->Update(toolBarWidth / 2 + toolBarWidth/16 * (i%8), 10);
            } else
            {
                buttons[i]->Update(toolBarWidth / 2 + toolBarWidth/16 * (i%8), 70);
            }
        } else
        {
            // Desenha Botões Gerais
            if (i < 5)
            {
                buttons[i]->Update(toolBarWidth/10 * (i%5), 10);
            } else
            {
                buttons[i]->Update(toolBarWidth/10 * (i%5), 70);
            }
        }
    }
}

void ToolBar::CreateButton(int height, int width, FunctionType functionType, char* name, float* rgb)
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


