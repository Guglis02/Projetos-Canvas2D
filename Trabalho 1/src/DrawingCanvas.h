#ifndef DRAWINGCANVAS_H_INCLUDED
#define DRAWINGCANVAS_H_INCLUDED

#include "Drawings/Drawing.h"
#include "MouseHandler.h"
#include "FileHandler.h"
#include "FunctionType.h"

using namespace std;

class DrawingCanvas
{
    public:
        DrawingCanvas()
        {
            LoadFromFile(drawings);
        }

    vector<Drawing*> drawings;
    vector<Vector2> tempPoints;

    Drawing* newDrawing = NULL;
    Drawing* selectedDrawing = NULL;

    float selectedColor[3] = {0, 0, 0};
    FunctionType currentFunction;

    Vector2 moveInc;

    // Callbacks
    void FillDrawing(void)
    {
        if (selectedDrawing)
        {
            selectedDrawing->SwitchFillable();
        }
    }

    void BringDrawingTop(void)
    {
        if (selectedDrawing && selectedDrawing != drawings.back())
        {
            iter_swap(find(drawings.begin(), drawings.end(), selectedDrawing),
                      find(drawings.begin(), drawings.end(), selectedDrawing) + 1);
        }
    }

    void SendDrawingBack(void)
    {
        if (selectedDrawing && selectedDrawing != drawings.front())
        {
            iter_swap(find(drawings.begin(), drawings.end(), selectedDrawing),
                      find(drawings.begin(), drawings.end(), selectedDrawing) - 1);
        }
    }

    void DeleteDrawing(void)
    {
        if (selectedDrawing)
        {
            drawings.erase(find(drawings.begin(), drawings.end(), selectedDrawing));
            delete selectedDrawing;
            selectedDrawing = NULL;
        }
    }

    void SaveFile(void)
    {
        SaveInFile(drawings);
    }

    void ClearCanvas(void)
    {
        drawings.clear();
        selectedDrawing = NULL;
    }
    //
    // Métodos da render
    const int circleIndicatorRadius = 5;
    void RenderPolygonPrototype()
    {
        int numberOfPoints = tempPoints.size();
        if (numberOfPoints != 0)
        {
            polygon(Vector2::GetXs(tempPoints.data(), numberOfPoints),
                    Vector2::GetYs(tempPoints.data(), numberOfPoints),
                    numberOfPoints);

            for (int i = 0; i < numberOfPoints; i++)
            {
                circle(tempPoints[i].x, tempPoints[i].y, circleIndicatorRadius, 10);
            }
        }
    }

    void Update(MouseHandler* mouseHandler, FunctionType currentFunction)
    {
        for (Drawing* d : drawings)
        {
            d->Render();
        }

        if (selectedDrawing)
        {
            selectedDrawing->RenderSelectionIndicators();
        }

        this->currentFunction = currentFunction;

        color(selectedColor[0],selectedColor[1],selectedColor[2]);
        if (newDrawing && mouseHandler->IsHolding())
        {
            newDrawing->RenderPrototype(mouseHandler->GetClickX(),
                                        mouseHandler->GetClickY(),
                                        mouseHandler->GetX(),
                                        mouseHandler->GetY());
        }

        if (currentFunction)
        {
            RenderPolygonPrototype();
        }

        // Usado apenas para controlar movimento do desenho com o teclado
        if (selectedDrawing && selectedDrawing->isMoving)
        {
            selectedDrawing->Move(moveInc);
        }
    }
    //

    void CheckDrawingSelection(int mx, int my)
    {
        for (Drawing* d : drawings)
        {
            if (d->CheckMouseClick(mx, my))
            {
                selectedDrawing = d;
                return;
            }
        }
        selectedDrawing = NULL;
    }

    void UpdateSelectedColor(float r, float g, float b)
    {
        selectedColor[0] = r;
        selectedColor[1] = g;
        selectedColor[2] = b;

        if (selectedDrawing)
        {
            selectedDrawing->SetColor(selectedColor[0],selectedColor[1],selectedColor[2]);
        }
    }

    void AddDrawing()
    {
        newDrawing->SetColor(selectedColor[0],selectedColor[1],selectedColor[2]);
        selectedDrawing = newDrawing;
        drawings.push_back(newDrawing);
        ResetNewDrawing();
    }

    void ResetNewDrawing()
    {
        newDrawing = NULL;
        tempPoints.clear();
    }

    void MouseClick(MouseHandler* mouseHandler)
    {
        switch(currentFunction)
        {
            case Rect:
                selectedDrawing = NULL;
                newDrawing = new RectangleDrawing(mouseHandler->GetClickX(),
                                                  mouseHandler->GetClickY(),
                                                  mouseHandler->GetX(),
                                                  mouseHandler->GetY());
                newDrawing->SetColor(selectedColor[0],selectedColor[1],selectedColor[2]);
                break;
            case Circle:
                selectedDrawing = NULL;
                newDrawing = new CircleDrawing(mouseHandler->GetClickX(),
                                               mouseHandler->GetClickY(),
                                               mouseHandler->GetDiffX(),
                                               32);
                newDrawing->SetColor(selectedColor[0],selectedColor[1],selectedColor[2]);
                break;
            case Triangle:
                selectedDrawing = NULL;
                newDrawing = new TriangleDrawing(mouseHandler->GetClickX(),
                                                 mouseHandler->GetClickY(),
                                                 mouseHandler->GetDiffX(),
                                                 mouseHandler->GetDiffY());
                newDrawing->SetColor(selectedColor[0],selectedColor[1],selectedColor[2]);
                break;
            case Poly:
                selectedDrawing = NULL;
                if (pnpoly(tempPoints.size(), tempPoints.data(), Vector2(mouseHandler->GetX(), mouseHandler->GetY())))
                {
                    newDrawing = new PolygonDrawing(Vector2::GetXs(tempPoints.data(), tempPoints.size()),
                                                    Vector2::GetYs(tempPoints.data(), tempPoints.size()),
                                                    tempPoints.size());
                    AddDrawing();
                } else
                {
                    tempPoints.push_back(Vector2(mouseHandler->GetX(), mouseHandler->GetY()));
                }
                break;
            default:
                if (selectedDrawing && selectedDrawing->CheckMouseInteraction(mouseHandler->GetX(), mouseHandler->GetY()))
                {
                    return;
                }
                CheckDrawingSelection(mouseHandler->GetX(), mouseHandler->GetY());
                break;
        }
    }

    void MouseDrag(MouseHandler* mouseHandler)
    {
        if (selectedDrawing)
        {
            selectedDrawing->EditDrawing(mouseHandler->GetX(),
                                         mouseHandler->GetY(),
                                         mouseHandler->GetDiffX(),
                                         mouseHandler->GetDiffY());
        }
    }

    void MouseRelease()
    {
        if (newDrawing && currentFunction != Poly)
        {
            AddDrawing();
        }
    }

    void Keyboard(int key)
    {
        if (key == 26 && !drawings.empty())
       {
            drawings.pop_back();
            selectedDrawing = NULL;
       }

       if (selectedDrawing)
       {
           switch(key)
           {
                case 200:
                    moveInc.set(-1, 0);
                    selectedDrawing->isMoving = true;
                    break;
                case 201:
                    moveInc.set(0, -1);
                    selectedDrawing->isMoving = true;
                    break;
                case 202:
                    moveInc.set(1, 0);
                    selectedDrawing->isMoving = true;
                    break;
                case 203:
                    moveInc.set(0, 1);
                    selectedDrawing->isMoving = true;
                    break;
                default:
                    break;
            }
        }
    }

    void KeyboardUp(int key)
    {
       if (selectedDrawing)
       {
            moveInc.set(0, 0);
            selectedDrawing->isMoving = false;
       }
    }
};

#endif // DRAWINGCANVAS_H_INCLUDED
