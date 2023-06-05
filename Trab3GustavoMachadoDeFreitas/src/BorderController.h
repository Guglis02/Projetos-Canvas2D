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

        //GenerateControlPoints(controlPoints);
    }

    void Update(float downSpeed)
    {
        for (int i = 0; i < controlPoints.size(); i++)
        {
            controlPoints[i].y -= FpsController::getInstance().normalize(downSpeed);
        }

        points.clear();
        for (int i = 0; i < controlPoints.size() - 3; i++)
        {
            for (float t = 0; t <= 1; t += 0.1f)
            {
                VectorHomo p = BSpline3(controlPoints[i], controlPoints[i + 1], controlPoints[i + 2], controlPoints[i + 3], t);
                points.push_back(p);
            }
        }

        DrawBorder(points);

        // for (int i = 0; i < controlPoints.size(); i++)
        // {
        //     CV::circleFill(controlPoints[i].x, controlPoints[i].y, 10, 5);
        //     if (controlPoints[i].y < 0 - screenHeight / 10)
        //     {
        //         controlPoints.erase(controlPoints.begin() + i);
        //         i--;
        //         VectorHomo p(rand() % 100 + x, rand() % 100 + screenHeight);
        //         controlPoints.push_back(p);
        //     }
        // }
    }
    private:

    int x;
    int screenWidth;
    int screenHeight;

    vector<VectorHomo> points;
    vector<VectorHomo> controlPoints;

    // void GenerateControlPoints(vector<VectorHomo>& points)
    // {
    //     int numberOfPoints = (screenHeight % 10) + 2;
    //     float distanceBetweenPoints = screenHeight / 10;
    //     float startingHeight = 0 - distanceBetweenPoints;

    //     for (int i = 0; i < numberOfPoints; i++)
    //     {
    //         VectorHomo p(rand() % 200 + (x - 100),
    //                     startingHeight + (distanceBetweenPoints * i));
    //         points.push_back(p);
    //     }
    // }


    void DrawBorder(vector<VectorHomo> points)
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
