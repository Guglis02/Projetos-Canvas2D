#ifndef UIMANAGER_H_INCLUDED
#define UIMANAGER_H_INCLUDED

#include "./FpsController.h"
#include "./GlobalConsts.h"
#include "../gl_canvas2d.h"
#include "./ViewMode.h"

// Classe responsável por gerenciar a UI do programa
class UIManager
{
public:
    UIManager()
    {    }

    // Atualiza os valores que devem ser mostrados na tela
    void Update(float rpm, float angleBetweenPistons, int numberOfEngines, float d, viewMode currentViewMode, int currentOption)
    {
        this->rpm = rpm;
        this->angleBetweenPistons = angleBetweenPistons;
        this->numberOfEngines = numberOfEngines;
        this->d = d;
        this->currentViewMode = currentViewMode;
        this->currentOption = currentOption;
    }

    // Formata e desenha na tela as informações do programa
    void Draw()
    {                
        CV::translate(0, 0);

        char auxLabel[64];
        sprintf(auxLabel, "FPS: %.1f", FpsController::getInstance().getFps());
        CV::color(0, 0, 0);
        CV::text(textX, 10 * textHeight, auxLabel);
        CV::text(textX, 9 * textHeight, "Para rotacionar utilize WASD nos eixos X e Y, e QE no eixo Z!");
        CV::text(textX, 8 * textHeight, "Para alterar a visualizacao use J, K e L!");
        sprintf(auxLabel, "Visualizacao: %s", ToString(currentViewMode));
        CV::text(textX, 7 * textHeight, auxLabel);
        CV::text(textX, 6 * textHeight, "Exibir: Camaras (c); Pistoes (p); Virabrequim (v);");
        sprintf(auxLabel, "Use o numero para selecionar e +/- para alterar: %s", GetCurrentOption());
        CV::text(textX, 5 * textHeight, auxLabel);
        sprintf(auxLabel, "1 - RPM: %.1f", rpm);
        CV::text(textX, 4 * textHeight, auxLabel);
        sprintf(auxLabel, "2 - Angulo entre pistoes: %.1f", angleBetweenPistons);
        CV::text(textX, 3 * textHeight, auxLabel);
        sprintf(auxLabel, "3 - Numero de motores: %d", numberOfEngines);
        CV::text(textX, 2 * textHeight, auxLabel);
        sprintf(auxLabel, "4 - Coeficiente d: %.1f", d);
        CV::text(textX, textHeight, auxLabel);
    }

    // FPS:
    // Para rotacionar utilize WASD nos eixos X e Y, e QE no eixo Z!

    // Para alterar a visualização use J, K e L!
    // Visualização: 2D
    // Exibir: Camaras (c) [ ]; Pistoes (p) [ ]; Virabrequim (v) [ ];
    // Use o número para selecionar e +/- para alterar:
    // 1 - RPM:
    // 2 - Angulo entre pistões:
    // 3 - Numero de motores:
    // 4 - Coeficiente d:

private:
    // Valores que serão desenhados na tela
    float rpm = 0;
    float angleBetweenPistons = 0;
    int numberOfEngines = 1;
    int currentOption = 0;
    float d = 0;
    viewMode currentViewMode;

    // Constantes auxiliares  
    float const textX = 15;
    float const textHeight = 15;

    char* GetCurrentOption()
    {
        switch (currentOption)
        {
        case 1:
            return "RPM";
        case 2:
            return "Angulo entre pistoes";
        case 3:
            return "Numero de motores";
        case 4:
            return "d";
        default:
            return " ";
        }
    }
};

#endif // UIMANAGER_H_INCLUDED
