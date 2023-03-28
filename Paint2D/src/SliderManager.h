#ifndef SLIDERMANAGER_H
#define SLIDERMANAGER_H


class SliderManager
{
    public:
        SliderManager(int x, int y, int barHeight, int barWidth);
        Update();


    private:
        int x;
        int y;

        int barHeight;
        int barWidth;

        int handleHeight;
        int handleWidth;
};

#endif // SLIDERMANAGER_H
