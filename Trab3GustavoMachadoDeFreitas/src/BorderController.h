#ifndef BORDERCONTROLLER_H_INCLUDED
#define BORDERCONTROLLER_H_INCLUDED

#include <vector>
#include "VectorHomo.h"
#include "gl_canvas2d.h"
#include "FpsController.h"

using namespace std;

class BorderController
{
    public:
    BorderController(int x, int screenWidth, int screenHeight)
    {
        this->x = x;
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;

        GenerateControlPoints(controlPoints);
    }

    void Update(float downSpeed)
    {
        for (int i = 0; i < controlPoints.size(); i++)
        {
            controlPoints[i].y -= FpsController::getInstance().normalize(downSpeed);
        }

        points.clear();
        for (int i = 0; i < controlPoints.size() - 4; i++)
        {
            for (float t = 0; t <= 1; t += 0.1f)
            {
                VectorHomo p = BSpline3(controlPoints[i], controlPoints[i + 1], controlPoints[i + 2], controlPoints[i + 3], t);
                if (p.y > 0 && p.y < screenHeight)
                {
                    points.push_back(p);
                }
            }
        }

        DrawBorder();

        for (int i = 0; i < controlPoints.size(); i++)
        {
            if (controlPoints[i].y < (0 - screenHeight >> 1))
            {
                controlPoints.erase(controlPoints.begin() + i);
                i--;
                VectorHomo p(rand() % 100 + x, screenHeight * 1.5);
                controlPoints.push_back(p);
            }
        }
    }

    vector<VectorHomo> points;

    private:

    int x;
    int screenWidth;
    int screenHeight;
    float distanceBetweenControlPoints = 100;

    vector<VectorHomo> controlPoints;

    void GenerateControlPoints(vector<VectorHomo>& cPoints)
    {
        int numberOfPoints = (screenHeight / distanceBetweenControlPoints) * 2.0;
        float startingHeight = 0 - screenHeight >> 1;

        for (int i = 0; i < numberOfPoints; i++)
        {
            VectorHomo p(rand() % 100 + (x - 50),
                        startingHeight + (distanceBetweenControlPoints * i));
            cPoints.push_back(p);
        }
    }

    void DrawBorder()
    {
        CV::color(5);
        for (int i = 0; i < points.size() - 1; i++)
        {
            CV::line(points[i], points[i + 1]);
        }
    }

    VectorHomo BSpline3(VectorHomo p1, VectorHomo p2, VectorHomo p3, VectorHomo p4, float t)
    {
        float asixth = 1.0f / 6.0f;
        float tcube = t * t * t;
        float tsquare = t * t;
        return p1 * asixth * pow((1 - t), 3)
            + p2 * asixth * (3 * tcube - 6 * tsquare + 4)
            + p3 * asixth * (-3 * tcube + 3 * tsquare + 3 * t + 1)
            + p4 * asixth * tcube;
    }
};

#endif // BORDERCONTROLLER_H_INCLUDED
