#ifndef FUNCTIONHANDLER_H_INCLUDED
#define FUNCTIONHANDLER_H_INCLUDED

#include <vector>
#include "Drawings/Drawing.h"

class FunctionHandler
{
    public:
        FunctionHandler(){}

        void FillDrawing(Drawing* drawing)
        {
            if (drawing)
            {
                drawing->SwitchFillable();
            }

        }

        void BringDrawingTop(Drawing* drawing, vector<Drawing*> drawings)
        {
            if (drawing && drawing != drawings.back())
            {
                iter_swap(find(drawings.begin(), drawings.end(), drawing),
                          find(drawings.begin(), drawings.end(), drawing) + 1);
            }
        }

        void SendDrawingBack(Drawing* drawing, vector<Drawing*> drawings)
        {
            if (drawing && drawing != drawings.front())
            {
                iter_swap(find(drawings.begin(), drawings.end(), drawing),
                          find(drawings.begin(), drawings.end(), drawing) - 1);
            }
        }

        void DeleteDrawing(Drawing* drawing, vector<Drawing*> drawings)
        {
            if (drawing)
            {
                drawings.erase(find(drawings.begin(), drawings.end(), drawing));
                delete drawing;
                drawing = NULL;
            }
        }

        void SaveDrawings(vector<Drawing*> drawings)
        {
            SaveInFile(drawings);
        }

        void DeleteAllDrawings(Drawing* drawing, vector<Drawing*> drawings)
        {
            drawings.clear();
            drawing = NULL;
        }
};

#endif // FUNCTIONHANDLER_H_INCLUDED
