#ifndef OBSTACLE_H_INCLUDED
#define OBSTACLE_H_INCLUDED

class Obstacle
{
    public:
        Obstacle(float x, float y, float gapTopPosition);

        float x;
        float y;

        void Update(float deltaTime);
        void Reset();
    private:
        const int gapHeight = 150;
        float gapTopPosition;
        float gapBottonPosition;

        const float moveSpeed = 3.0/50;
        const float width = 30;

        void Draw();
        void Move(float deltaTime);
};

#endif // OBSTACLE_H_INCLUDED
