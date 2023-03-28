#ifndef LAYERSDISPLAY_H
#define LAYERSDISPLAY_H

#include "gl_canvas2d.h"

class LayersDisplay
{
    public:
        LayersDisplay(int displayHeight, int displayWidth);
        void Update(int screenHeight, int screenWidth);
    private:
        int displayHeight;
        int displayWidth;

};

#endif // LAYERSDISPLAY_H
