#ifndef __VECTORHOMO3D_H__
#define __VECTORHOMO3D_H__

#include <stdio.h>
#include <math.h>

// Classe que representa um vetor 3d homogêneo.

class VectorHomo3d
{
public:
    float x, y, z, w;

    VectorHomo3d()
    {
        x = y = z = 0;
        w = 1;
    }

    VectorHomo3d(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
        w = 1;
    }

    void set(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    void set(VectorHomo3d vec)
    {
        x = vec.x;
        y = vec.y;
        z = vec.z;
        w = vec.w;
    }

    //	Soma de vetores
    inline VectorHomo3d operator+(const VectorHomo3d v)
    {
        VectorHomo3d aux(x + v.x, y + v.y, z + v.z);
        return (aux);
    }

    //	Subtracao de vetores
    inline VectorHomo3d operator-(const VectorHomo3d v)
    {
        VectorHomo3d aux(x - v.x, y - v.y, z - v.z);
        return (aux);
    }

    //	Produto por escalar (float)
    inline VectorHomo3d operator*(const float s)
    {
        VectorHomo3d aux(x * s, y * s, z * s);
        return (aux);
    }

    //	Divisao por escalar
    inline VectorHomo3d operator/(const float s)
    {
        VectorHomo3d aux(x / s, y / s, z / s);
        return (aux);
    }

    //	Produto escalar
    inline float operator*(const VectorHomo3d v)
    {
        float aux = x * v.x + y * v.y + z * v.z;
        return (aux);
    }

    //	Produto vetorial
    inline VectorHomo3d operator^(VectorHomo3d v)
    {
        VectorHomo3d aux(y * v.z - z * v.y,
                         z * v.x - x * v.z,
                         x * v.y - y * v.x);
        return (aux);
    }

    void operator+=(const VectorHomo3d &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
    }

    bool operator==(const VectorHomo3d &v)
    {
        return (x == v.x && y == v.y && z == v.z);
    }

    float norm()
    {
        return (float)sqrt(x * x + y * y + z * z);
    }

    float distance(VectorHomo3d v)
    {
        return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z));
    }

    void normalize()
    {
        float n = norm();

        if (n == 0.0)
        {
            printf("\n\nNormalize::Divisao por zero");
            x = 1;
            y = 1;
            z = 1;
            return;
        }
        x /= n;
        y /= n;
        z /= n;
    }
};

#endif
