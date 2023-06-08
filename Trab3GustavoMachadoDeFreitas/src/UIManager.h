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
        RenderFpsCounter();
        RenderPlayerScore();
    }

    void SetPlayerScore(int playerScore)
    {
        this->uiPlayerScore = playerScore;
    }

private:
    int uiPlayerScore = 0;

    void RenderFpsCounter()
    {
        char fpsLabel[64];
        sprintf(fpsLabel, "FPS: %.1f", FpsController::getInstance().getFps());
        CV::color(2);
        CV::text(100, ConstScreenHeight - 50, fpsLabel);
    }

    void RenderPlayerScore()
    {
        char playerScoreLabel[64];
        sprintf(playerScoreLabel, "Score: %d", uiPlayerScore);
        CV::color(2);
        CV::text(ConstScreenWidth >> 1, ConstScreenHeight - 50, playerScoreLabel);
    }
};

#endif // UIMANAGER_H_INCLUDED
