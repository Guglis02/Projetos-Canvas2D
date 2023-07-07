#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "../Utils/VectorHomo3d.h"
#include "../Utils/Matrix3d.h"

using namespace std;

// Classe base dos modelos 3D usados no programa
class Model
{
public:
    Model(VectorHomo3d center, int steps)
    {
        this->center = center;
        this->steps = steps;
        this->points.resize(steps);
        for (int i = 0; i < steps; i++)
        {
            this->points[i].resize(steps);
        }
        transformationMatrix = new Matrix3d();
    }
    ~Model()
    {
        delete transformationMatrix;
    }

    // Rotaciona o modelo em torno do seu centro
    // A rotacao pode alterar permanentemente os pontos ou apenas alterar os pontos temporários.
    void LocalRotate(float anglex, float angley, float anglez, bool isPerm)
    {
        LocalRotate(anglex, angley, anglez, isPerm, center);
    }

    // Rotaciona o modelo em torno de uma ancora.
    // A rotacao pode alterar permanentemente os pontos ou apenas alterar os pontos temporários.
    void LocalRotate(float anglex, float angley, float anglez, bool isPerm, VectorHomo3d anchor)
    {
        transformationMatrix->Reset();
        transformationMatrix->Translation(anchor);
        transformationMatrix->RotationX(anglex);
        transformationMatrix->RotationY(angley);
        transformationMatrix->RotationZ(anglez);
        transformationMatrix->Translation(anchor * -1);

        transformedPoints = transformationMatrix->ApplyToPoints(transformedPoints);

        if (isPerm)
        {
            points = transformationMatrix->ApplyToPoints(points);
        }
    }

    // Reposiciona o modelo no mundo, pode ser permanente ou não.
    void Reposition(VectorHomo3d newCenter, bool isPerm)
    {
        transformationMatrix->Reset();
        transformationMatrix->Translation(center * -1);
        center = newCenter;
        transformationMatrix->Translation(newCenter);

        transformedPoints = transformationMatrix->ApplyToPoints(points);

        if (isPerm)
        {
            points = transformationMatrix->ApplyToPoints(points);
        }
    }

    void ResetTransformedPoints()
    {
        transformedPoints = points;
    }

    // Rotaciona globalmente o modelo em torno da origem
    void GlobalRotate(float anglex, float angley, float anglez)
    {
        transformationMatrix->Reset();
        transformationMatrix->RotationX(DegToRad(anglex));
        transformationMatrix->RotationY(DegToRad(angley));
        transformationMatrix->RotationZ(DegToRad(anglez));

        transformedPoints = transformationMatrix->ApplyToPoints(transformedPoints);
    }

    // Pontos do modelo
    vector<vector<VectorHomo3d>> points;

    virtual void Build(){};

    // Desenha o modelo em perspectiva
    void DrawPerspective(int d)
    {
        for (unsigned int i = 0; i < transformedPoints.size(); i++)
        {
            for (unsigned int j = 0; j < transformedPoints[i].size(); j++)
            {
                int x = i;
                int y = j;
                int nx = (i + 1) % (transformedPoints.size());
                int ny = (j + 1) % (transformedPoints[i].size());

                CV::perspectiveLine(transformedPoints[x][y], transformedPoints[x][nx], d);
                CV::perspectiveLine(transformedPoints[x][y], transformedPoints[nx][y], d);
                CV::perspectiveLine(transformedPoints[x][y], transformedPoints[nx][ny], d);
            }
        }
    }

    // Desenha o modelo com camera ortogonal
    void DrawOrthogonal()
    {
        for (int i = 0; i < transformedPoints.size(); i++)
        {
            for (int j = 0; j < transformedPoints[i].size(); j++)
            {
                int x = i;
                int y = j;
                int nx = (i + 1) % (transformedPoints.size());
                int ny = (j + 1) % (transformedPoints[i].size());
                CV::line(transformedPoints[x][y], transformedPoints[x][nx]);
                CV::line(transformedPoints[x][y], transformedPoints[nx][y]);
                CV::line(transformedPoints[x][y], transformedPoints[nx][ny]);
            }
        }
    }

protected:
    Matrix3d *transformationMatrix;
    vector<vector<VectorHomo3d>> transformedPoints;
    VectorHomo3d center;
    int steps = 20;
};

#endif
