#ifndef MOUSEHANDLER_H_INCLUDED
#define MOUSEHANDLER_H_INCLUDED

class MouseHandler
{
    public:
        MouseHandler(){}

        void Update(int button, int state, int wheel, int direction, int x, int y)
        {
            this->button = button;
            this->state = state;
            this->wheel = wheel;
            this->direction = direction;
            this->x = x;
            this->y = y;

            if (state == 0)
            {
                isHolding = true;
            } else if (state == 1)
            {
                isHolding = false;
            }
        }

        bool IsPointerUnder(int y)
        {
            return this->y > y;
        }

        int button;
        int state;
        int wheel;
        int direction;
        int x;
        int y;

        bool isHolding = false;
};

#endif // MOUSEHANDLER_H_INCLUDED
