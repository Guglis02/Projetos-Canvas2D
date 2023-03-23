#include "ToolBar.h"
#include "gl_canvas2d.h"

ToolBar::ToolBar(int toolBarHeight)
{
    this->toolBarHeight = toolBarHeight;
    StartButtons();
}

void ToolBar::StartButtons()
{
   Button* pencilButton = new Button(80, 80, Pencil, "Lapis");
   buttons.push_back(pencilButton);

   Button* rectButton = new Button(80, 80, Rect, "Retangulo");
   buttons.push_back(rectButton);

   Button* clearButton = new Button(80,80, Clear, "Limpar");
   buttons.push_back(clearButton);
}

void ToolBar::Update(int screenHeight, int screenWidth)
{
    color(1);
    rectFill(0, 0, screenWidth, toolBarHeight);

    for (int i = 0; i < buttons.size(); i++)
    {
        buttons[i]->Update(screenWidth/5 * (i + 1), 50);
    }
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


