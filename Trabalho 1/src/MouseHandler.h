#ifndef MOUSEHANDLER_H_INCLUDED
#define MOUSEHANDLER_H_INCLUDED

/** \brief
 * Classe que lida com as propriedades do mouse,
 * além de gerenciar algumas flags úteis.
 */
class MouseHandler
{
public:
    MouseHandler(void)
    {
        this->x = this->oldX = this->clickX = 0;
        this->y = this->oldY = this->clickY = 0;
    }

    // Atualiza atributos da classe.
    void Update(int button, int state, int wheel, int direction, int x, int y)
    {
        this->button = button;
        this->state = state;
        this->wheel = wheel;
        this->direction = direction;
        this->oldX = this->x;
        this->oldY = this->y;
        this->x = x;
        this->y = y;

        if (state == 0)
        {
            isHolding = true;
            this->clickX = x;
            this->clickY = y;
        }
        else if (state == 1)
        {
            isHolding = false;
            isDragging = false;
        }
        else if (isHolding)
        {
            isDragging = true;
        }
    }

    float GetX(void)
    {
        return this->x;
    }
    float GetY(void)
    {
        return this->y;
    }

    float GetDiffX(void)
    {
        return this->x - this->oldX;
    }
    float GetDiffY(void)
    {
        return this->y - this->oldY;
    }

    float GetClickX(void)
    {
        return this->clickX;
    }
    float GetClickY(void)
    {
        return this->clickY;
    }

    bool IsHolding(void)
    {
        return this->isHolding;
    }
    bool IsDragging(void)
    {
        return this->isDragging;
    }

    int GetState(void)
    {
        return this->state;
    }

private:
    int button;
    int state;
    int wheel;
    int direction;

    float clickX;
    float clickY;
    float oldX;
    float oldY;
    float x;
    float y;

    bool isHolding = false;
    bool isDragging = false;
};

#endif // MOUSEHANDLER_H_INCLUDED
