#ifndef __VECTOR_2_H__
#define __VECTOR_2_H__

#include <stdio.h>
#include <math.h>

class Vector2
{
public:
   float x, y;

   Vector2()
   {
      x = y = 0;
   }

   Vector2(float _x, float _y)
   {
       x = _x;
       y = _y;
   }

   void set(float _x, float _y)
   {
       x = _x;
       y = _y;
   }

    void set(Vector2 vec)
    {
        x = vec.x;
        y = vec.y;
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

   static float DotProduct(Vector2 vec1, Vector2 vec2)
   {
       return vec1.x * vec2.x + vec1.y * vec2.y;
   }

   static float CrossProduct(Vector2 vec1, Vector2 vec2)
   {
       return vec1.x * vec2.y - vec1.y * vec2.x;
   }

   static float* GetXs(Vector2* vectors, int size)
   {
        float* xs = new float[size];

        for (int i = 0; i < size; i++)
        {
            xs[i] = vectors[i].x;
        }

        return xs;
   }

    static float* GetYs(Vector2* vectors, int size)
   {
        float* ys = new float[size];

        for (int i = 0; i < size; i++)
        {
            ys[i] = vectors[i].y;
        }

        return ys;
   }

   Vector2 operator - (const Vector2& v)
   {
        Vector2 aux( x - v.x, y - v.y);
        return( aux );
   }

   Vector2 operator + (const Vector2& v)
   {
       Vector2 aux( x + v.x, y + v.y);
       return( aux );
   }
   //Adicionem os demais overloads de operadores aqui.

    Vector2 operator += (const Vector2& v)
    {
       Vector2 aux( x += v.x, y += v.y);
       return( aux );
    }

    Vector2 operator * (const float& f)
    {
       Vector2 aux( x * f, y * f);
       return( aux );
    }

    Vector2 operator / (const float& f)
    {
       Vector2 aux( x / f, y / f);
       return( aux );
    }

};

#endif
