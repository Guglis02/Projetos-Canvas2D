#ifndef FILEHANDLER_H_INCLUDED
#define FILEHANDLER_H_INCLUDED

#include <iostream>
#include <fstream>
#include "Drawings/Drawing.h"
#include "Drawings/CircleDrawing.h"
#include "Drawings/PolygonDrawing.h"
#include "Drawings/RectangleDrawing.h"
#include "Drawings/TriangleDrawing.h"
#include "FunctionType.h"

using namespace std;

void SaveInFile(vector<Drawing*> drawings)
{
    FILE* file = fopen("figuras.gr", "w");

    if (!file)
    {
        return;
    }

    int numberOfDrawings = drawings.size();

    fprintf(file, "%d\n", numberOfDrawings);

    for (Drawing* d : drawings)
    {
        FunctionType type = d->GetType();

        int elementsCounter;

        fprintf(file, "%d ", type);
        printf("\n FunctionType = %d", type);
        switch (type)
        {
        case Rect:
            fprintf(file, "%.f %.f %.f %.f ",
                    d->GetAnchor().x,
                    d->GetAnchor().y,
                    d->GetWidth(),
                    d->GetHeight());
            break;
        case Circle:
            fprintf(file, "%.f %.f %.f ",
                    d->GetAnchor().x,
                    d->GetAnchor().y,
                    d->GetWidth() / 2);
            break;
        case Triangle:
            fprintf(file, "%.f %.f %.f %.f ",
                    d->GetAnchor().x,
                    d->GetAnchor().y,
                    d->GetWidth(),
                    d->GetHeight());
            break;
        case Poly:
            elementsCounter = d->GetElementsCount();
            fprintf(file, "%d ", elementsCounter);

            for (int i = 0; i < elementsCounter; i++)
            {
                fprintf(file, "%.f %.f ",
                        d->GetOriginPoints()[i].x + d->GetAnchor().x,
                        d->GetOriginPoints()[i].y + d->GetAnchor().y);
            }
            break;
        default:
            exit(1);
            break;
        }

        float* color = d->GetColor();
        bool fillFlag = d->GetFillFlag();
        float angle = d->GetAngle();
        Vector2 proportion = d->GetProportion();

        fprintf(file, "%.2f %.2f %.2f %d %.2f %.2f %.2f\n",
                color[0],
                color[1],
                color[2],
                fillFlag,
                angle,
                proportion.x,
                proportion.y);
    }

    fclose(file);
}

void LoadFromFile(vector<Drawing*>&drawings)
{
    std::ifstream file("figuras.gr");

    if (!file.is_open())
    {
        return;
    }

    if (file.peek() == EOF)
    {
        return;
    }

    int numberOfDrawings;
    file >> numberOfDrawings;

    Drawing* drawing;
    FunctionType type;

    int typeIndex;
    float x1, y1, width, height, radius;
    int elementsCounter;

    for (int i = 0; i < numberOfDrawings; i++)
    {
        file >> typeIndex;

        type = static_cast<FunctionType>(typeIndex);

        switch (type)
        {
        case Rect:
            file >> x1 >> y1 >> width >> height;
            drawing = new RectangleDrawing(x1, y1, x1 + width, y1 + height);
            break;
        case Triangle:
            file >> x1 >> y1 >> width >> height;
            drawing = new TriangleDrawing(x1, y1, width, height);
            break;
        case Circle:
            file >> x1 >> y1 >> radius;
            drawing = new CircleDrawing(x1, y1, radius, 32);
            break;
        case Poly:
        {
            file >> elementsCounter;

            float* xs = new float[elementsCounter];
            float* ys = new float[elementsCounter];

            for (int i = 0; i < elementsCounter; i++)
            {
                file >> xs[i] >> ys[i];
            }

            drawing = new PolygonDrawing(xs, ys, elementsCounter);
            delete[] xs;
            delete[] ys;

            break;
        }
        default:
            exit(1);
            break;
        }

        float r, g, b;
        bool fillFlag;
        float angle;
        float proportionX;
        float proportionY;

        file >> r >> g >> b >> fillFlag >> angle >> proportionX >> proportionY;
        drawing->SetColor(r, g, b);
        drawing->SetFillFlag(fillFlag);
        drawing->LoadProportion(Vector2(proportionX, proportionY));
        drawing->ApplyAngle(angle);

        drawings.push_back(drawing);
    }

    file.close();
}


#endif // FILEHANDLER_H_INCLUDED
