#ifndef BORDERCONTROLLER_H_INCLUDED
#define BORDERCONTROLLER_H_INCLUDED

#include <vector>
#include "./Utils/VectorHomo.h"
#include "./Utils/CurveUtils.h"
#include "./Utils/PointsUtils.h"
#include "./Utils/GlobalConsts.h"
#include "./Utils/FpsController.h"

using namespace std;

// Classe responsável por controlar a geração e movimentação de uma borda
class BorderController
{
public:
    BorderController(int x)
    {
        this->x = x;

        GenerateControlPoints();
    }

    void Update(float downSpeed)
    {
        for (unsigned int i = 0; i < controlPoints.size(); i++)
        {
            controlPoints[i].y -= FpsController::getInstance().normalize(downSpeed);
        }

        CalculateCurvePoints();
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
    float const startingHeight = 0 - (ConstScreenHeight >> 1);

    vector<VectorHomo> controlPoints;

    // Gera os pontos de controle da curva
    void GenerateControlPoints()
    {
        int numberOfPoints = (ConstScreenHeight / distanceBetweenControlPoints) * 2.0;

        for (int i = 0; i < numberOfPoints; i++)
        {
            VectorHomo p(rand() % 100 + (x - 50),
                         startingHeight + (distanceBetweenControlPoints * i));
            controlPoints.push_back(p);
        }
    }

    // Calcula os pontos da curva
    void CalculateCurvePoints()
    {
        points.clear();
        for (unsigned int i = 0; i < controlPoints.size() - 3; i++)
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

    // Deleta os pontos de controle que estão fora da tela
    // e cria novos pontos de controle para substituir os deletados
    void DeleteOutOfBoundsControlPoints()
    {
        for (unsigned int i = 0; i < controlPoints.size(); i++)
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
};

#endif // BORDERCONTROLLER_H_INCLUDED
