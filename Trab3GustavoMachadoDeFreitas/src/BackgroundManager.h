#ifndef BACKGROUNDMANAGER_H_INCLUDED
#define BACKGROUNDMANAGER_H_INCLUDED

#include "BorderController.h"

// Classe responsável por gerenciar o background e as bordas do jogo
class BackgroundManager
{
public:
    BackgroundManager(int borderWidth)
    {
        this->leftBorder = new BorderController(borderWidth);
        this->rightBorder = new BorderController(ConstScreenWidth - borderWidth);

        // Importa Bmps
        Bmp *upperTrenchBmp = new Bmp("./Trab3GustavoMachadoDeFreitas/images/UpperTrench.bmp");
        upperTrenchBmp->convertBGRtoRGB();
        this->upperTrenchTexture = upperTrenchBmp->getImage();
        Bmp *lowerTrenchBmp = new Bmp("./Trab3GustavoMachadoDeFreitas/images/LowerTrench.bmp");
        lowerTrenchBmp->convertBGRtoRGB();
        this->lowerTrenchTexture = lowerTrenchBmp->getImage();

        this->texturesWidth = upperTrenchBmp->getWidth();
        this->texturesHeight = upperTrenchBmp->getHeight();
    }

    void Update(int downSpeed)
    {
        // "Desce" o background de acordo uma velocidade
        leftBorder->Update(downSpeed);
        rightBorder->Update(downSpeed);

        if (qualityMode)
        {
            PaintBackgroundWithDots();
        }
        else
        {
            PaintBackgroundWithLines();
        }
    }

    void ToggleQualityMode()
    {
        qualityMode = !qualityMode;
    }

    bool CheckLeftCollision(vector<VectorHomo> hitbox)
    {
        return leftBorder->CheckCollision(hitbox);
    }

    bool CheckRightCollision(vector<VectorHomo> hitbox)
    {
        return rightBorder->CheckCollision(hitbox);
    }

    BorderController *leftBorder = NULL;
    BorderController *rightBorder = NULL;

private:
    int borderWidth;

    int texturesWidth;
    int texturesHeight;
    uchar *upperTrenchTexture = NULL;
    uchar *lowerTrenchTexture = NULL;

    bool qualityMode = false;

    int lastH = 0;
    int textureIterator = 0;
    // Pinta o background com as texturas importadas
    // levando em consideração as bordas
    void PaintBackgroundWithDots()
    {
        if (lowerTrenchTexture == nullptr || upperTrenchTexture == nullptr)
        {
            return;
        }

        // Para cada ponto amostrado da borda
        for (unsigned int i = 0; i < leftBorder->points.size(); i++)
        {
            VectorHomo leftPoint = leftBorder->points[i];
            VectorHomo rightPoint = rightBorder->points[i];

            // Para cada linha entre o ponto anterior e o atual
            for (int h = lastH; h < leftPoint.y; h++)
            {
                // Para cada pixel da linha
                for (int j = 0; j < ConstScreenWidth * 3; j += 3)
                {
                    textureIterator = textureIterator % texturesHeight;
                    int pos = textureIterator * ((3 * (texturesWidth + 1) / 4) * 4) + j;
                    int pixelX = j / 3;

                    if (pixelX > leftPoint.x && pixelX < rightPoint.x)
                    {
                        CV::color(
                            (float)lowerTrenchTexture[pos] / 255.0,
                            (float)lowerTrenchTexture[pos + 1] / 255.0,
                            (float)lowerTrenchTexture[pos + 2] / 255.0);
                    }
                    else
                    {
                        CV::color(
                            (float)upperTrenchTexture[pos] / 255.0,
                            (float)upperTrenchTexture[pos + 1] / 255.0,
                            (float)upperTrenchTexture[pos + 2] / 255.0);
                    }

                    CV::point(pixelX, h);
                }
                textureIterator++;
            }

            lastH = leftPoint.y;
        }
    }

    void PaintBackgroundWithLines()
    {
        for (unsigned int i = 0; i < leftBorder->points.size(); i++)
        {
            VectorHomo leftPoint = leftBorder->points[i];
            VectorHomo rightPoint = rightBorder->points[i];

            for (int h = lastH; h < leftPoint.y; h++)
            {
                CV::color(0.180, 0.220, 0.259);
                CV::line(0, h, leftPoint.x, h);
                CV::line(rightPoint.x, h, ConstScreenWidth, h);

                CV::color(0.098, 0.125, 0.157);
                CV::line(leftPoint.x, h, rightPoint.x, h);
            }

            lastH = leftPoint.y;
        }
    }
};

#endif // BACKGROUNDMANAGER_H_INCLUDED
