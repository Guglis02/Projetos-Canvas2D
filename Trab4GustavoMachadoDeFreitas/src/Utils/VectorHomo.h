#ifndef __VECTORHOMO_H__
#define __VECTORHOMO_H__

#include <stdio.h>
#include <math.h>

// Classe que representa um vetor 2d homogêneo.

class VectorHomo
{
public:
    float x, y, z;

    VectorHomo()
    {
        x = y = 0;
        z = 1;
    }

    VectorHomo(float _x, float _y)
    {
        x = _x;
        y = _y;
        z = 1;
    }

    VectorHomo(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    void set(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

    void set(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    void set(VectorHomo vec)
    {
        x = vec.x;
        y = vec.y;
        z = vec.z;
    }

    void normalize()
    {
        float norm = (float)sqrt(x*x + y*y);

        if(norm==0.0)
        {
            printf("\n\nNormalize::Divisao por zero");
            x = 1;
            y = 1;
            return;
        }
        x /= norm;
        y /= norm;
    }

    VectorHomo rotate(float angle)
    {
        float x1 = x;
        float y1 = y;

        x = x1 * cos(angle) - y1 * sin(angle);
        y = x1 * sin(angle) + y1 * cos(angle);

        return *this;
    }

    static float DotProduct(VectorHomo vec1, VectorHomo vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }

    static float CrossProduct(VectorHomo vec1, VectorHomo vec2)
    {
        return vec1.x * vec2.y - vec1.y * vec2.x;
    }

    static float* GetXs(VectorHomo* vectors, int size)
    {
        float* xs = new float[size];

        for (int i = 0; i < size; i++)
        {
            xs[i] = vectors[i].x;
        }

        return xs;
    }

    static float* GetYs(VectorHomo* vectors, int size)
    {
        float* ys = new float[size];

        for (int i = 0; i < size; i++)
        {
            ys[i] = vectors[i].y;
        }

        return ys;
    }

    VectorHomo operator - (const VectorHomo& v)
    {
        VectorHomo aux( x - v.x, y - v.y);
        return( aux );
    }

    VectorHomo operator + (const VectorHomo& v)
    {
        VectorHomo aux( x + v.x, y + v.y);
        return( aux );
    }
    //Adicionem os demais overloads de operadores aqui.

    VectorHomo operator += (const VectorHomo& v)
    {
        VectorHomo aux( x += v.x, y += v.y);
        return( aux );
    }

    VectorHomo operator * (const float& f)
    {
        VectorHomo aux( x * f, y * f);
        return( aux );
    }

    VectorHomo operator / (const float& f)
    {
        VectorHomo aux( x / f, y / f);
        return( aux );
    }

    bool operator == (const VectorHomo& v)
    {
        return (x == v.x && y == v.y && z == v.z);
    }
};

#endif
