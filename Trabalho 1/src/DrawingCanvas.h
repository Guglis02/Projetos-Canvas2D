#ifndef DRAWINGCANVAS_H_INCLUDED
#define DRAWINGCANVAS_H_INCLUDED

#include "Drawings/Drawing.h"
#include "MouseHandler.h"
#include "FileHandler.h"
#include "FunctionType.h"
#include "Color.h"

using namespace std;

/** \brief
 * Classe responsável pelo canvas de desenho,
 * portanto gerencia boa parte da lógica do programa.
 */
class DrawingCanvas
{
public:
    DrawingCanvas(function<void()> deselectButtonCallback, function<void(Color)> refreshColorCallback)
    {
        LoadFromFile(drawings);
        this->deselectButtonCallback = deselectButtonCallback;
        this->refreshColorCallback = refreshColorCallback;
    }

    // Callbacks
    void FillDrawing(void)
    {
        if (selectedDrawing)
        {
            selectedDrawing->SwitchFillable();
        }
        deselectButtonCallback();
    }

    void BringDrawingTop(void)
    {
        if (selectedDrawing && selectedDrawing != drawings.back())
        {
            iter_swap(find(drawings.begin(), drawings.end(), selectedDrawing),
                      find(drawings.begin(), drawings.end(), selectedDrawing) + 1);
        }
        deselectButtonCallback();
    }

    void SendDrawingBack(void)
    {
        if (selectedDrawing && selectedDrawing != drawings.front())
        {
            iter_swap(find(drawings.begin(), drawings.end(), selectedDrawing),
                      find(drawings.begin(), drawings.end(), selectedDrawing) - 1);
        }
        deselectButtonCallback();
    }

    void DeleteDrawing(void)
    {
        if (selectedDrawing)
        {
            drawings.erase(find(drawings.begin(), drawings.end(), selectedDrawing));
            delete selectedDrawing;
            selectedDrawing = NULL;
        }
        deselectButtonCallback();
    }

    void SaveFile(void)
    {
        SaveInFile(drawings);
        deselectButtonCallback();
    }

    void ClearCanvas(void)
    {
        drawings.clear();
        selectedDrawing = NULL;
        deselectButtonCallback();
    }
    /////////////

    // Método chamado em todo frame
    void Update(MouseHandler* mouseHandler, FunctionType currentFunction)
    {
        // Desenha os desenhos da lista
        for (Drawing* d : drawings)
        {
            d->Render();
        }
        // Se um desenho estiver selecionado, desenha os indicadores
        if (selectedDrawing)
        {
            selectedDrawing->RenderSelectionIndicators();
        }

        this->currentFunction = currentFunction;

        // Se o usuário estiver desenhando, desenha o protótipo na tela
        color(selectedColor.r,selectedColor.g,selectedColor.b, selectedColor.alpha);
        if (newDrawing && mouseHandler->IsHolding())
        {
            newDrawing->RenderPrototype(mouseHandler->GetClickX(),
                                        mouseHandler->GetClickY(),
                                        mouseHandler->GetX(),
                                        mouseHandler->GetY());
        }
        // Como os polígonos livres são inseridos ponto a ponto
        // este método é chamado caso seja a função selecionada
        if (currentFunction == Poly)
        {
            RenderPolygonPrototype();
        }

        // Usado apenas para controlar movimento do desenho com o teclado
        if (selectedDrawing && selectedDrawing->isMoving)
        {
            selectedDrawing->Move(moveInc);
        }
    }
    // Reseta desenho que está sendo criado
    void ResetNewDrawing(void)
    {
        newDrawing = NULL;
        tempPoints.clear();
    }
    // Atualiza cor selecionada
    void UpdateSelectedColor(Color rgb)
    {
        selectedColor = rgb;

        if (selectedDrawing)
        {
            selectedDrawing->SetColor(rgb);
        }
    }

    void OnMouseClick(MouseHandler* mouseHandler)
    {
        switch(currentFunction)
        {
        case Rect:
            selectedDrawing = NULL;
            newDrawing = new RectangleDrawing(mouseHandler->GetClickX(),
                                              mouseHandler->GetClickY(),
                                              mouseHandler->GetX(),
                                              mouseHandler->GetY());
            newDrawing->SetColor(selectedColor);
            break;
        case Circle:
            selectedDrawing = NULL;
            newDrawing = new CircleDrawing(mouseHandler->GetClickX(),
                                           mouseHandler->GetClickY(),
                                           mouseHandler->GetDiffX(),
                                           32);
            newDrawing->SetColor(selectedColor);
            break;
        case Triangle:
            selectedDrawing = NULL;
            newDrawing = new TriangleDrawing(mouseHandler->GetClickX(),
                                             mouseHandler->GetClickY(),
                                             mouseHandler->GetDiffX(),
                                             mouseHandler->GetDiffY());
            newDrawing->SetColor(selectedColor);
            break;
        case Poly:
            selectedDrawing = NULL;
            // Caso o clique seja dentro do protótipo do polígono, o desenho é concluído
            if (pnpoly(tempPoints.size(), tempPoints.data(), Vector2(mouseHandler->GetX(), mouseHandler->GetY())))
            {
                newDrawing = new PolygonDrawing(Vector2::GetXs(tempPoints.data(), tempPoints.size()),
                                                Vector2::GetYs(tempPoints.data(), tempPoints.size()),
                                                tempPoints.size());
                AddDrawing();
            }
            // Senão, outro ponto é adicionado ao protótipo
            else
            {
                tempPoints.push_back(Vector2(mouseHandler->GetX(), mouseHandler->GetY()));
            }
            break;
        default:
            // Se um desenho está selecionado, checa os pontos de interação
            if (selectedDrawing && selectedDrawing->CheckMouseInteraction(mouseHandler->GetX(), mouseHandler->GetY()))
            {
                return;
            }
            // Checa se está clicando em um desenho
            CheckDrawingSelection(mouseHandler->GetX(), mouseHandler->GetY());
            break;
        }
    }

    void OnMouseDrag(MouseHandler* mouseHandler)
    {
        if (selectedDrawing)
        {
            selectedDrawing->EditDrawing(mouseHandler->GetX(),
                                         mouseHandler->GetY(),
                                         mouseHandler->GetDiffX(),
                                         mouseHandler->GetDiffY());
        }
    }

    void OnMouseRelease(void)
    {
        if (!newDrawing)
        {
            return;
        }

        // Impede a criação de um desenho muito pequeno
        bool isSizeValid = newDrawing->GetHeight() > minDrawingSize
                           || newDrawing->GetWidth() > minDrawingSize;

        if (isSizeValid && currentFunction != Poly)
        {
            AddDrawing();
        }
    }

    void OnKeyboardDown(int key)
    {
        // Ctrl+Z vai removendo os desenhos, do último ao primeiro
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

    void OnKeyboardUp(int key)
    {
        if (selectedDrawing)
        {
            moveInc.set(0, 0);
            selectedDrawing->isMoving = false;
        }
    }
private:
    const int circleIndicatorRadius = 5;
    const int minDrawingSize = 5;

    vector<Drawing*> drawings;
    vector<Vector2> tempPoints;

    Drawing* newDrawing = NULL;
    Drawing* selectedDrawing = NULL;

    function<void()> deselectButtonCallback;
    function<void(Color)> refreshColorCallback;

    Color selectedColor = Color();
    FunctionType currentFunction;

    Vector2 moveInc;

    // Desenha o protótipo do polígono
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

    // Adiciona um novo desenho a lista de desenhos a serem renderizados
    void AddDrawing()
    {
        newDrawing->SetColor(selectedColor);
        selectedDrawing = newDrawing;
        drawings.push_back(newDrawing);
        deselectButtonCallback();
        ResetNewDrawing();
    }

    // Checa o clique do mouse nos desenhos, priorizando o mais a frente no canvas.
    // Caso um desenho seja selecionado, atualiza o seletor de cor para corresponder ao desenho.
    void CheckDrawingSelection(int mx, int my)
    {
        Drawing* temp = NULL;

        for (Drawing* d : drawings)
        {
            if (d->CheckMouseClick(mx, my))
            {
                temp = d;
            }
        }
        selectedDrawing = temp;

        if (selectedDrawing)
        {
            refreshColorCallback(selectedDrawing->GetColor());
        }
    }
};

#endif // DRAWINGCANVAS_H_INCLUDED
