#include "LayersDisplay.h"

LayersDisplay::LayersDisplay(int displayHeight, int displayWidth)
{
    this->displayHeight = displayHeight;
    this->displayWidth = displayWidth;
}

void LayersDisplay::Update(int screenHeight, int screenWidth)
{
    color(1);
    rectFill(screenWidth - displayWidth, 0, screenWidth, displayHeight);
}

