#ifndef MATRIX3D_H_INCLUDED
#define MATRIX3D_H_INCLUDED

#include <vector>
#include "VectorHomo3d.h"

using namespace std;

// Arquivo com funções para cálculo de matrizes homogêneas 3d
class Matrix3d
{
public:
    Matrix3d()
    {
        Reset();
    }
    ~Matrix3d() {}

    // Constroi uma matriz identidade
    vector<vector<float>> BuildIdentity()
    {
        vector<vector<float>> aux;

        aux.resize(4);
        for (int i = 0; i < 4; i++)
        {
            aux[i].resize(4);
            for (int j = 0; j < 4; j++)
            {
                aux[i][j] = i == j ? 1.0f : 0.0f;
            }
        }

        return (aux);
    }

    void Translation(VectorHomo3d v)
    {
        Translation(v.x, v.y, v.z);
    }
    void Translation(float x, float y, float z)
    {
        vector<vector<float>> aux = BuildIdentity();
        aux[0][3] = x;
        aux[1][3] = y;
        aux[2][3] = z;
        ApplyToSelf(aux);
    }

    void RotationX(float angle)
    {
        vector<vector<float>> aux = BuildIdentity();
        aux[1][1] = cos(angle);
        aux[1][2] = -1 * sin(angle);
        aux[2][1] = sin(angle);
        aux[2][2] = cos(angle);
        ApplyToSelf(aux);
    }

    void RotationY(float angle)
    {
        vector<vector<float>> aux = BuildIdentity();
        aux[0][0] = cos(angle);
        aux[0][2] = sin(angle);
        aux[2][0] = -1 * sin(angle);
        aux[2][2] = cos(angle);
        ApplyToSelf(aux);
    }

    void RotationZ(float angle)
    {
        vector<vector<float>> aux = BuildIdentity();
        aux[0][0] = cos(angle);
        aux[0][1] = -1 * sin(angle);
        aux[1][0] = sin(angle);
        aux[1][1] = cos(angle);
        ApplyToSelf(aux);
    }

    void Scale(float x = 1, float y = 1, float z = 1)
    {
        vector<vector<float>> aux = BuildIdentity();
        aux[0][0] = x;
        aux[1][1] = y;
        aux[2][2] = z;
        ApplyToSelf(aux);
    }

    // Multiplica a matriz atual por um conjunto de pontos
    vector<VectorHomo3d *> ApplyToPoints(vector<VectorHomo3d *> points)
    {
        vector<VectorHomo3d *> transformedPoints;
        for (auto point : points)
        {
            VectorHomo3d *transformedPoint = new VectorHomo3d();
            *transformedPoint = *this * (*point);
            transformedPoints.push_back(transformedPoint);
        }
        return transformedPoints;
    }

    vector<VectorHomo3d> ApplyToPoints(vector<VectorHomo3d> points)
    {
        vector<VectorHomo3d> transformedPoints;
        for (auto point : points)
        {
            VectorHomo3d transformedPoint = *this * (point);
            transformedPoints.push_back(transformedPoint);
        }
        return transformedPoints;
    }

    vector<vector<VectorHomo3d>> ApplyToPoints(vector<vector<VectorHomo3d>> points)
    {
        vector<vector<VectorHomo3d>> transformedPoints;
        for (auto point : points)
        {
            transformedPoints.push_back(ApplyToPoints(point));
        }
        return transformedPoints;
    }

    void Print()
    {
        printf("\n");
        for (int i = 0; i < 4; i++)
        {
            printf("\n");
            for (int j = 0; j < 4; j++)
            {
                printf("%f  ", matrix[i][j]);
            }
        }
    }

    void Reset()
    {
        matrix = BuildIdentity();
    }

    VectorHomo3d operator*(const VectorHomo3d &v)
    {
        VectorHomo3d aux;

        aux.x = matrix[0][0] * v.x + matrix[0][1] * v.y + matrix[0][2] * v.z + matrix[0][3] * v.w;
        aux.y = matrix[1][0] * v.x + matrix[1][1] * v.y + matrix[1][2] * v.z + matrix[1][3] * v.w;
        aux.z = matrix[2][0] * v.x + matrix[2][1] * v.y + matrix[2][2] * v.z + matrix[2][3] * v.w;
        aux.w = matrix[3][0] * v.x + matrix[3][1] * v.y + matrix[3][2] * v.z + matrix[3][3] * v.w;

        return (aux);
    }

private:
    vector<vector<float>> matrix;

    // Aplica transformação em si mesma, usada para concatenar operações
    void ApplyToSelf(vector<vector<float>> transformation)
    {
        vector<vector<float>> aux = BuildIdentity();

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < transformation.size(); j++)
            {
                aux[i][j] = 0;
                for (int k = 0; k < 4; k++)
                {
                    aux[i][j] += matrix[i][k] * transformation[k][j];
                }
            }
        }

        matrix = aux;
    }
};

#endif // MATRIX3D_H_INCLUDED
