#ifndef OBSTACLE_H
#define OBSTACLE_H

class Obstacle
{
    public:
        Obstacle(float x, float y, float gapTopPosition);

        float x;
        float y;

        void Update(float deltaTime);
    private:
        const int gapHeight = 50;
        float gapTopPosition;
        float gapBottonPosition;

        const float moveSpeed = 3/1000;
        const float width = 30;

        void Draw();
        void Move(float deltaTime);
};

#endif // OBSTACLE_H
