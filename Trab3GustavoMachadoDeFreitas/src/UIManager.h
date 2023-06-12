#ifndef UIMANAGER_H_INCLUDED
#define UIMANAGER_H_INCLUDED

#include "./Utils/FpsController.h"
#include "./Utils/GlobalConsts.h"
#include "gl_canvas2d.h"
#include "GameManager.h"

// Classe responsável por gerenciar a UI do jogo
class UIManager
{
public:
    UIManager()
    {
    }

    void Update()
    {
        RenderUpperBar();
        RenderScores();
        RenderClock();
        RenderLowerBar();
        RenderPlayerHP();
        RenderFpsCounter();
    }

    void DrawStartScreen()
    {
        CV::color(2);
        CentralizeTextX(halfHeight, "Press E to start endless mode");
        CentralizeTextX(halfHeight - 15, "Press T to start 200 points run");
        CentralizeTextX(halfHeight - 30, "Press Q to toggle game quality");
    }

    void DrawGameOverScreen()
    {
        char auxLabel[64];
        CV::color(2);
        sprintf(auxLabel, "Your scored %d points in %.1f seconds!", uiPlayerScore, uiTime);
        CentralizeTextX(halfHeight - 15, auxLabel);
        CentralizeTextX(halfHeight - 30, "Press E to start endless mode");
        sprintf(auxLabel, "Press T to start the %d points run", TargetPoints);
        CentralizeTextX(halfHeight - 45, auxLabel);
    }

    void SetHighScore(int highScore)
    {
        this->uiHighScore = highScore;
    }

    void SetPlayerScore(int playerScore)
    {
        this->uiPlayerScore = playerScore;
    }

    void SetTime(float seconds)
    {
        this->uiTime = seconds;
    }

    void SetPlayerHP(int playerHP)
    {
        this->uiPlayerHP = playerHP;
    }

    void SetPlayerMaxHP(int playerMaxHP)
    {
        this->uiPlayerMaxHP = playerMaxHP;
    }

private:
    int uiPlayerScore = 0;
    int uiHighScore = 0;
    int uiPlayerHP = 0;
    int uiPlayerMaxHP = 0;
    float uiTime = 0;

    float halfWidth = ConstScreenWidth >> 1;
    float halfHeight = ConstScreenHeight >> 1;

    void RenderUpperBar()
    {
        CV::color(1);
        CV::rectFill(0, ConstScreenHeight, ConstScreenWidth, ConstScreenHeight - 20);
    }

    void RenderFpsCounter()
    {
        char fpsLabel[64];
        sprintf(fpsLabel, "FPS: %.1f", FpsController::getInstance().getFps());
        CV::color(2);
        CV::text(ConstScreenWidth - 100, 5, fpsLabel);
    }

    void RenderScores()
    {
        char scoreLabel[64];
        CV::color(2);
        sprintf(scoreLabel, "High Score: %d", uiHighScore);
        CV::text(10, ConstScreenHeight - 15, scoreLabel);
        sprintf(scoreLabel, "Score: %d", uiPlayerScore);
        CentralizeTextX(ConstScreenHeight - 15, scoreLabel);
    }

    void RenderClock()
    {
        char clockLabel[64];
        sprintf(clockLabel, "Time: %.1f", uiTime);
        CV::color(2);
        CV::text(ConstScreenWidth - 200, ConstScreenHeight - 15, clockLabel);
    }

    void RenderLowerBar()
    {
        CV::color(1);
        CV::rectFill(0, 0, ConstScreenWidth, 20);
    }

    void RenderPlayerHP()
    {
        float iconSize = 16;
        VectorHomo drawingPosition;

        for (int i = 1; i <= uiPlayerMaxHP; i++)
        {
            drawingPosition = VectorHomo(2 * iconSize * (i + 1), iconSize);
            if (i <= uiPlayerHP)
            {
                DrawMillenniumFalcon(drawingPosition, 0.5);
            }
            else
            {
                DrawMillenniumFalconWireframe(drawingPosition, 0.5);
            }
        }
    }

    void CentralizeTextX(int textY, const char *text)
    {
        float textX = halfWidth - (strlen(text) * ApproximateFontSize) / 2;

        CV::color(2);
        CV::text(textX, textY, text);
    }
};

#endif // UIMANAGER_H_INCLUDED
