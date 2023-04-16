#ifndef COLORBAR_H_INCLUDED
#define COLORBAR_H_INCLUDED

#include "ColorUtils.h"
#include "Interactables/Slider.h"
#include "Bars/HueSlider.h"
#include "Bars/SatSlider.h"
#include "Bars/ValueSlider.h"
#include <vector>

/** \brief
Classe responsável pela barra de escolha de cores.
Gerencia as barras de Hue, Saturation e Value.
Mostra um preview da cor selecionada.
 */
class ColorBar : public Bar
{
public:
    ColorBar(int height, int width) : Bar(height, width)
    {
        hueSlider = new HueSlider(0, 0, width/3, height/3, 0.0, 360, 0.5);
        saturationSlider = new SatSlider(0, 0, width/3, height/3, 0.0, 1.0, 0.5);
        valueSlider = new ValueSlider(0, 0, width/3, height/3, 0.0, 1.0, 0.5);
        sliders.push_back(hueSlider);
        sliders.push_back(saturationSlider);
        sliders.push_back(valueSlider);
    }

    void Update(int x, int y, int height, int width)
    {
        Bar::Update(x, y, height, width);

        hueSlider->Update(x, y, width/2, height/3);
        saturationSlider->Update(x, y + height/3, width/2, height/3);
        valueSlider->Update(x, y + 2 * height/3, width/2, height/3);
        // Desenha as barras
        hueSlider->Render(saturationSlider->GetValue(), valueSlider->GetValue());
        saturationSlider->Render(hueSlider->GetValue(), valueSlider->GetValue());
        valueSlider->Render(hueSlider->GetValue(), saturationSlider->GetValue());

        // Desenha preview da cor selecionada
        DrawPreview();

        UpdateColor();
    }

    // Retorna o valor selecionado nas barras em formato RGB.
    void GetRGB(float& r, float& g, float& b)
    {
        HSVtoRGB(hueSlider->GetValue(),
                 saturationSlider->GetValue(),
                 valueSlider->GetValue(),
                 r, g, b);
    }

    // Recebe um valor em formato RGB, converte pra HSV e chama atualização das barras.
    void SetRGB(float* rgb)
    {
        RGBtoHSV(rgb[0], rgb[1], rgb[2], this->hue, this->saturation, this->value);
        UpdateBars();
    }

    void OnMouseClick(int mx, int my)
    {
        for (Slider* slider : sliders)
        {
            if (slider->IsMouseInside(mx, my))
            {
                slider->OnMouseClick(mx, my);
            }
        }
    }

    void OnMouseDrag(int mx, int my, function<void(float*)> callback)
    {
        for (Slider* slider : sliders)
        {
            if (slider->IsMouseInside(mx, my))
            {
                slider->OnMouseDrag(mx);
            }
        }

        float* rgb = new float[3];
        GetRGB(rgb[0], rgb[1], rgb[2]);
        callback(rgb);
    }

    void OnMouseRelease(int mx, int my)
    {
        for (Slider* slider : sliders)
        {
            if (slider->IsMouseInside(mx, my))
            {
                slider->OnMouseRelease();
            }
        }
    }
private:
    float hue = 0;
    float saturation = 0;
    float value = 0;

    // Desenha preview da cor selecionada
    void DrawPreview(void)
    {
        float r, g, b;
        GetRGB(r, g, b);

        color(r, g, b);
        rectFill(x + width * 0.5, y, x + width, y + height);
    }

    // Atualiza cor selecionada
    void UpdateColor(void)
    {
        float hue = hueSlider->GetValue() * 360;
        float saturation = saturationSlider->GetValue() * 100;
        float value = valueSlider->GetValue();

        SetHSV(hue, saturation, value);
    }

    // Atualiza barras de cor
    void UpdateBars()
    {
        hueSlider->SetValue(hue);
        saturationSlider->SetValue(saturation);
        valueSlider->SetValue(value);
    }

    // Setter dos valores
    void SetHSV(float hue, float saturation, float value)
    {
        this->hue = hue;
        this->saturation = saturation;
        this->value = value;
    }

    HueSlider* hueSlider = NULL;
    SatSlider* saturationSlider = NULL;
    ValueSlider* valueSlider = NULL;

    vector<Slider*> sliders;
};

#endif // COLORBAR_H_INCLUDED
