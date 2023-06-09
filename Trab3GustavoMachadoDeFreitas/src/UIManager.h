#ifndef UIMANAGER_H_INCLUDED
#define UIMANAGER_H_INCLUDED

#include "./Utils/FpsController.h"
#include "./Utils/GlobalConsts.h"
#include "gl_canvas2d.h"

class UIManager
{
public:
    UIManager()
    {
    }

    void Update()
    {
        RenderUpperBar();
        RenderFpsCounter();
        RenderPlayerScore();
        RenderLowerBar();
        RenderPlayerHP();
    }

    void SetPlayerScore(int playerScore)
    {
        this->uiPlayerScore = playerScore;
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
    int uiPlayerHP = 0;
    int uiPlayerMaxHP = 0;

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
        CV::text(50, ConstScreenHeight - 15, fpsLabel);
    }

    void RenderPlayerScore()
    {
        char playerScoreLabel[64];
        sprintf(playerScoreLabel, "Score: %d", uiPlayerScore);
        CV::color(2);
        CV::text(ConstScreenWidth >> 1, ConstScreenHeight - 15, playerScoreLabel);
    }

    void RenderLowerBar()
    {
        CV::color(1);
        CV::rectFill(0, 0, ConstScreenWidth, 20);
    }

    void RenderPlayerHP()
    {
        char playerHPLabel[64];
        sprintf(playerHPLabel, "HP: %d/%d", uiPlayerHP, uiPlayerMaxHP);
        CV::color(2);
        CV::text(ConstScreenWidth - 100, 10, playerHPLabel);
    }
};

#endif // UIMANAGER_H_INCLUDED
