#ifndef _FRAMES_H___
#define _FRAMES_H___

#include <stdio.h>
#include <time.h>

#define UPDATE_RATE 30 // milisegundos

// Baseada na classe do professor, porém transformada em um Singleton
class FpsController
{
private:
    clock_t t1, t2;
    long frameCounter;
    float fps, fps_old;

    FpsController()
    {
        t1 = clock();
        frameCounter = 0;
        fps_old = 20;
    }

public:
    // Singleton
    static FpsController &getInstance()
    {
        static FpsController instance;
        return instance;
    }

    // Atualiza o contador de frames
    void updateFrames()
    {
        double timeWindow;

        t2 = clock();
        timeWindow = (double)(t2 - t1);
        frameCounter++;

        if (timeWindow > UPDATE_RATE)
        {
            t1 = t2;
            fps = frameCounter / (timeWindow / CLOCKS_PER_SEC);
            frameCounter = 0;
        }
    }

    float getFps()
    {
        return fps;
    }

    // Normaliza o valor de acordo com o FPS
    float normalize(float value)
    {
        if (fps == 0)
        {
            return 0;
        }
        float deltaTime = 1.0f / fps;
        return value * deltaTime;
    }

    double GetDeltaTime()
    {
        if (fps == 0)
        {
            return 0;
        }
        return 1.0f / fps;
    }
};

#endif
