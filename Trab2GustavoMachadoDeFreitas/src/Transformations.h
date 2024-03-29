#ifndef TRANSFORMATIONS_H_INCLUDED
#define TRANSFORMATIONS_H_INCLUDED

#include <math.h>
#include <vector>

#include "gl_canvas2d.h"

using namespace std;

/** \brief
 * Classe que cont�m as fun��es respons�veis
 * por aplicar DCT, IDCT e calcular a diferen�a entre o input e os valores restaurados.
 * A l�gica dos c�digos foi fortemente baseada nas apresentadas na demo 16 do Canvas2D.
 */
class Transformations
{
public:
    static double C (int w)
    {
        if (w==0)
        {
            return 0.707106781186;  /* 1/sqrt(2) */
        }
        return 1.0;
    }

    static vector<double> DiscreteCosineTransform(vector<double> input, double quantizationCoefficient)
    {
        double val, coef;
        int vectorSize = input.size();

        vector<double> output(vectorSize);

        for(int i = 0; i < vectorSize; i++)
        {
            val = 0.0f;
            coef = C(i);

            for(int j = 0; j < vectorSize; j++)
            {
                val += input[j] * cos(((2.0 * j) + 1.0) * i * PI / (2.0 * vectorSize));
            }

            val = val * coef;

            val = val * sqrt(2.0/vectorSize);

            if (quantizationCoefficient != 0)
            {
                val = floor(val / quantizationCoefficient) * quantizationCoefficient;
            }

            output[i] = (val);
        }

        return output;
    }

    static vector<double> InverseDiscreteCosineTransform(vector<double> input)
    {
        double val, coef;
        int vectorSize = input.size();

        vector<double> output(vectorSize);

        for(int i = 0; i < vectorSize; i++)
        {
            val = 0.0f;

            for(int j = 0; j < vectorSize; j++)
            {
                coef = C(j);
                val += input[j] * cos(((2.0 * i) + 1.0) * j * PI / (2.0 * vectorSize)) * coef;
            }

            output[i] = (val * sqrt(2.0/vectorSize));
        }

        return output;
    }

    static vector<double> CalculateDiff(vector<double> v1, vector<double> v2)
    {
        vector<double> diff;

        for(int i = 0; i < v1.size(); i++)
        {
            diff.push_back(v1[i] - v2[i]);
        }

        return diff;
    }
};

#endif // TRANSFORMATIONS_H_INCLUDED
