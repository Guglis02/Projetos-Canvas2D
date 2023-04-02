#ifndef POLYGONDRAWING_H_INCLUDED
#define POLYGONDRAWING_H_INCLUDED

#include <limits>
#include <algorithm>
#include "Drawing.h"

using namespace std;

class PolygonDrawing : public Drawing
{
    public:
        PolygonDrawing(int xs[], int ys[], int elementsCounter)
        {
            this->type = Poly;

            this->elementsCounter = elementsCounter;

            this->xs = new float[this->elementsCounter];
            this->ys = new float[this->elementsCounter];

            // Variaveis auxiliares para pegar os pontos extremos e montar o indicador de seleção
            int minX = numeric_limits<int>::max();
            int minY = numeric_limits<int>::max();
            int maxX = numeric_limits<int>::lowest();
            int maxY = numeric_limits<int>::lowest();

            for(int i = 0; i < elementsCounter; i++)
            {
                minX = min(xs[i], minX);
                minY = min(ys[i], minY);
                maxX = max(xs[i], maxX);
                maxY = max(ys[i], maxY);

                this->AddPoint(xs[i], ys[i], i);
            }

            this->AddSelectionPoint(minX, minY, 0);
            this->AddSelectionPoint(maxX, minY, 1);
            this->AddSelectionPoint(maxX, maxY, 2);
            this->AddSelectionPoint(minX, maxY, 3);
        }
};

#endif // POLYGONDRAWING_H_INCLUDED
