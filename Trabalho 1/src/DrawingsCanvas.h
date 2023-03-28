#ifndef DRAWINGSCANVAS_H_INCLUDED
#define DRAWINGSCANVAS_H_INCLUDED

class DrawingsCanvas
{
    public:
        DrawingsCanvas(int x, int y, int height, int width);

        void Update(int height, int width);
    private:
        int x = 0;
        int y = 0;
        int height = 0;
        int width = 0;
};

#endif // DRAWINGSCANVAS_H_INCLUDED
