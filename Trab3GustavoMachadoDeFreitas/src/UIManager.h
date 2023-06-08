#ifndef UIMANAGER_H_INCLUDED
#define UIMANAGER_H_INCLUDED

#include "./Utils/FpsController.h"
#include "gl_canvas2d.h"

class UIManager
{
public:
    UIManager(int screenWidth, int screenHeight)
    {
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;
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
    int screenWidth;
    int screenHeight;

    int uiPlayerScore = 0;

    void RenderFpsCounter()
    {
        char fpsLabel[64];
        sprintf(fpsLabel, "FPS: %.1f", FpsController::getInstance().getFps());
        CV::color(2);
        CV::text(100, screenHeight - 200, fpsLabel);
    }

    void RenderPlayerScore()
    {
        char playerScoreLabel[64];
        sprintf(playerScoreLabel, "Score: %d", uiPlayerScore);
        CV::color(2);
        CV::text(screenWidth >> 1, screenHeight - 150, playerScoreLabel);
    }
};

#endif // UIMANAGER_H_INCLUDED
