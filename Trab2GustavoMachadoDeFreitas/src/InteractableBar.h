#ifndef INTERACTABLEBAR_H_INCLUDED
#define INTERACTABLEBAR_H_INCLUDED

#include <functional>
#include "Interactables/Button.h"
#include "Interactables/Slider.h"

using namespace std;

/** \brief
 * Classe responsável por gerenciar a barra de elementos interagíveis na interface.
 */
class InteractableBar
{
public:
    InteractableBar(int height, int width)
    {
        this->height = height;
        this->width = width;
    }

    // Atualiza dimensões da barra e de seus elementos
    void Update(int height, int width)
    {
        this->width = width;
        this->height = height;
        CV::color(1);
        CV::rectFill(x, y, x + width, height);

        int elementsCount = elements.size();
        int halfRoundedCount = (elementsCount + 1) >> 1;

        // Dispõe os elementos em duas linhas.
        for (int i = 0; i < elementsCount; i++)
        {
            int interactableSpace = x + width/halfRoundedCount;
            if (i < halfRoundedCount)
            {
                elements[i]->Update(leftBorder + interactableSpace * (i%elementsCount), 60);
            }
            else
            {
                elements[i]->Update(leftBorder + interactableSpace * (i%halfRoundedCount), 5);
            }
            elements[i]->Render();
        }
    }

    // Cria um botão e adiciona ele na lista.
    void CreateButton(const char* name, const char* desc, function<void()> callback)
    {
        Button* newButton = new Button(defaultButtonWidth, defaultButtonHeight,
                                       callback, name, desc);
        newButton->SetColor(defaultButtonColor);
        elements.push_back(newButton);
    }

    // Cria um slider e adiciona ele na lista.
    void CreateSlider(const char* name, float minVal, float maxVal, float initialValue, function<void(float)> callback)
    {
        Slider* newSlider = new Slider(defaultSliderWidth, defaultSliderHeight,
                                       minVal, maxVal, initialValue, name, callback);
        elements.push_back(newSlider);
    }

    void CheckMouseClick(int mx, int my)
    {
        for (Interactable* b : elements)
        {
            if (b->IsMouseInside(mx, my))
            {
                b->OnClick();
            }
        }
    }

    void OnMouseDrag(int mx)
    {
        for (Interactable* b : elements)
        {
            b->OnDrag(mx);
        }
    }

    void OnMouseRelease(void)
    {
        for (Interactable* b : elements)
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

    vector<Interactable*> elements;
};

#endif // INTERACTABLEBAR_H_INCLUDED
