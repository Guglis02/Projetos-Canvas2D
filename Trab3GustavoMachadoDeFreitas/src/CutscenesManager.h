#ifndef CUTSCENESMANAGER_H_INCLUDED
#define CUTSCENESMANAGER_H_INCLUDED

#include "./Bmp/Bmp.h"
#include <vector>

// Classe responsável por gerenciar as cutscenes do jogo
class CutscenesManager
{
public:
    CutscenesManager()
    {
        char path[100];
        Bmp *bmp = NULL;

        // Importa Bmps e salva informações em vetores
        for (int i = 1; i <= numberOfFrames; i++)
        {
            sprintf(path, "./Trab3GustavoMachadoDeFreitas/images/GameOverFrames/GameOver%d.bmp", i);
            bmp = new Bmp(path);
            this->texturesWidth = bmp->getWidth();
            this->texturesHeight = bmp->getHeight();
            bmp->convertBGRtoRGB();
            gameOverFrames.push_back(bmp->getImage());

            sprintf(path, "./Trab3GustavoMachadoDeFreitas/images/VictoryFrames/Victory%d.bmp", i);
            bmp = new Bmp(path);
            bmp->convertBGRtoRGB();
            victoryFrames.push_back(bmp->getImage());
        }
    }

    void GameOverCutscene()
    {
        PlayCutscene(gameOverFrames);
    }

    void VictoryCutscene()
    {
        PlayCutscene(victoryFrames);
    }

private:
    const int numberOfFrames = 18;

    int texturesWidth;
    int texturesHeight;

    float timeSinceLastFrame = 0;
    unsigned int currentFrame = 0;

    vector<uchar *> gameOverFrames;
    vector<uchar *> victoryFrames;

    // Recebe um vetor de frames e renderiza cada um deles
    // ao longo do tempo.
    void PlayCutscene(vector<uchar *> &frames)
    {
        timeSinceLastFrame += FpsController::getInstance().GetDeltaTime();

        if (timeSinceLastFrame > 1.0 / 8)
        {
            currentFrame++;
            timeSinceLastFrame = 0;
        }

        if (currentFrame >= frames.size())
        {
            currentFrame = 0;
        }

        RenderFrame(frames[currentFrame]);
    }

    // Renderiza um frame no centro da tela
    void RenderFrame(uchar *frame)
    {
        if (frame == NULL)
        {
            return;
        }

        int bytesPerLine = (3 * (texturesWidth + 1) / 4) * 4;
        int yOffset = (ConstScreenHeight) / 2;
        int xOffset = (ConstScreenWidth - texturesWidth) / 2;

        for (int y = 0; y < texturesHeight; y++)
        {
            for (int x = 0; x < texturesWidth * 3; x += 3)
            {
                int pixelX = x / 3;
                int pos = y * bytesPerLine + x;
                CV::color(
                    (float)frame[pos] / 255.0,
                    (float)frame[pos + 1] / 255.0,
                    (float)frame[pos + 2] / 255.0);

                CV::point(pixelX + xOffset,
                          y + yOffset);
            }
        }
    }
};

#endif // CUTSCENESMANAGER_H_INCLUDED
