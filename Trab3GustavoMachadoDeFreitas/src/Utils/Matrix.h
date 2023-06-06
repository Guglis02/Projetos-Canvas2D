#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <vector>
#include "VectorHomo.h"

using namespace std;

class Matrix {
    public:
        Matrix()
        {
            Reset();
        }

        vector<vector<float>> BuildIdentity()
        {
            vector<vector<float>> aux;

            aux.resize(3);
            for (int i = 0; i < 3; i++)
            {
                aux[i].resize(3);
                for (int j = 0; j < 3; j++)
                {
                    aux[i][j] = i == j ? 1.0f : 0.0f;
                }
            }

            return ( aux );
        }

        void ApplyToSelf(vector<vector<float>> transformation)
        {
            vector<vector<float>> aux = BuildIdentity();

            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    aux[i][j] = 0.0f;
                    for (int k = 0; k < 3; k++)
                    {
                        aux[i][j] += matrix[i][k] * transformation[k][j];
                    }
                }
            }

            matrix = aux;
        }

        void Translation(float x, float y)
        {
            vector<vector<float>> aux = BuildIdentity();
            aux[0][2] = x;
            aux[1][2] = y;
            ApplyToSelf(aux);
        }

        void Rotation(float angle)
        {
            vector<vector<float>> aux = BuildIdentity();
            aux[0][0] = cos(angle);
            aux[0][1] = -1 * sin(angle);
            aux[1][0] = sin(angle);
            aux[1][1] = cos(angle);
            ApplyToSelf(aux);
        }

        void Scale(float x, float y)
        {
            vector<vector<float>> aux = BuildIdentity();
            aux[0][0] = x;
            aux[1][1] = y;
            ApplyToSelf(aux);
        }

        vector<VectorHomo> ApplyToPoints(vector<VectorHomo> points)
        {
            vector<VectorHomo> aux;
            for (auto point : points)
            {
                aux.push_back(*this * point);
            }
            return (aux);
        }

        void Print()
        {
            for(int i = 0; i < 3; i++){
                printf("\n");
                for(int j = 0; j < 3; j++){
                    printf("%f  ", matrix[i][j]);
                }
            }
        }

        void Reset()
        {
            matrix = BuildIdentity();
        }

        VectorHomo operator * (const VectorHomo& v)
        {
            VectorHomo aux( matrix[0][0] * v.x + matrix[0][1] * v.y + matrix[0][2] * v.z,
                         matrix[1][0] * v.x + matrix[1][1] * v.y + matrix[1][2] * v.z,
                         matrix[2][0] * v.x + matrix[2][1] * v.y + matrix[2][2] * v.z);
            return( aux );
        }
    private:
        vector<vector<float>> matrix;
};

#endif // MATRIX_H_INCLUDED
