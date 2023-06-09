#ifndef BORDERCONTROLLER_H_INCLUDED
#define BORDERCONTROLLER_H_INCLUDED

#include <vector>
#include "./Utils/VectorHomo.h"
#include "./Utils/CurveUtils.h"
#include "./Utils/PointsUtils.h"
#include "./Utils/GlobalConsts.h"
#include "gl_canvas2d.h"
#include "./Utils/FpsController.h"

using namespace std;

class BorderController
{
    public:
    BorderController(int x)
    {
        this->x = x;

        GenerateControlPoints(controlPoints);
    }

    void Update(float downSpeed)
    {
        for (int i = 0; i < controlPoints.size(); i++)
        {
            controlPoints[i].y -= FpsController::getInstance().normalize(downSpeed);
        }

        CalculateCurvePoints();
        DrawBorder();
        DeleteOutOfBoundsControlPoints();
    }

    bool CheckCollision(vector<VectorHomo> hitbox)
    {
        for (auto point : points)
        {
            if (pnpoly(hitbox.size(), hitbox.data(), point))
            {
                return true;
            }
        }

        return false;
    }

    int GetX()
    {
        return x;
    }

    vector<VectorHomo> points;

    private:

    int x;
    float const distanceBetweenControlPoints = 100;
    float const startingHeight = 0 - ConstScreenHeight >> 1;

    vector<VectorHomo> controlPoints;

    void GenerateControlPoints(vector<VectorHomo>& cPoints)
    {
        int numberOfPoints = (ConstScreenHeight / distanceBetweenControlPoints) * 2.0;

        for (int i = 0; i < numberOfPoints; i++)
        {
            VectorHomo p(rand() % 100 + (x - 50),
                        startingHeight + (distanceBetweenControlPoints * i));
            cPoints.push_back(p);
        }
    }

    void CalculateCurvePoints()
    {
        points.clear();
        for (int i = 0; i < controlPoints.size() - 3; i++)
        {
            for (float t = 0; t <= 1; t += 0.1f)
            {
                VectorHomo p = BSpline3(controlPoints[i], controlPoints[i + 1], controlPoints[i + 2], controlPoints[i + 3], t);
                if (!IsOutOfBounds(p, 1))
                {
                    points.push_back(p);
                }
            }
        }
    }

    void DeleteOutOfBoundsControlPoints()
    {
        for (int i = 0; i < controlPoints.size(); i++)
        {
            if (controlPoints[i].y < startingHeight)
            {
                controlPoints.erase(controlPoints.begin() + i);
                i--;
                VectorHomo p(rand() % 100 + (x - 50), ConstScreenHeight * 1.5);
                controlPoints.push_back(p);
            }
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
};

#endif // BORDERCONTROLLER_H_INCLUDED
