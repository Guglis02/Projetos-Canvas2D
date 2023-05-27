#ifndef _FRAMES_H___
#define _FRAMES_H___

#include <stdio.h>
#include <time.h>

#define UPDATE_RATE 30   //milisegundos

// Baseada na classe do professor, porém transformada em um Singleton
class FpsController {
private:
    clock_t t1, t2;
    long frameCounter;
    float fps, fps_old;

    FpsController() {
        t1 = clock();
        frameCounter = 0;
        fps_old = 20;
    }

public:
    static FpsController& getInstance() {
        static FpsController instance;
        return instance;
    }

    void getFrames() {
        double timeWindow;

        t2 = clock();
        timeWindow = (double)(t2 - t1);
        frameCounter++;

        if (timeWindow > UPDATE_RATE) {
            t1 = t2;
            fps = frameCounter / (timeWindow / CLOCKS_PER_SEC);
            frameCounter = 0;
        }
    }

    float getFps()
    {
        return fps;
    }

    float normalize(float value)
    {
        float deltaTime = 1.0f / fps;
        return value * deltaTime;
    }

    double GetDeltaTime()
    {
        return 1.0f / fps;
    }
};

#endif
