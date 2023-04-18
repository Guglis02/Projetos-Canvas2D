#ifndef FILEHANDLER_H_INCLUDED
#define FILEHANDLER_H_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>
#include "Drawings/Drawing.h"
#include "Drawings/CircleDrawing.h"
#include "Drawings/PolygonDrawing.h"
#include "Drawings/RectangleDrawing.h"
#include "Drawings/TriangleDrawing.h"
#include "FunctionType.h"

using namespace std;

/** \brief
 * Arquivo utilitário contendo funções relacionadas a lidar com arquivos.
 * Possui métodos auxiliares que usam cifra de ceasar para criptografar o arquivo de figuras.
 */

const int EncryptionKey = 10;

string Encrypt(string str, int key)
{
    string encrypted = str;
    for (char& chr : encrypted)
    {
        chr = ((chr + key) % 127);
    }
    return encrypted;
}

string Decrypt(string str, int key)
{
    string decrypted = str;
    for (char& chr : decrypted)
    {
        chr = ((chr - key) % 127);
    }
    return decrypted;
}

// Recebe a lista de desenhos e decompõe cada um em uma série de atributos
void SaveInFile(vector<Drawing*> drawings)
{
    std::stringstream buffer;

    int numberOfDrawings = drawings.size();

    buffer << numberOfDrawings << std::endl;

    for (Drawing* d : drawings)
    {
        FunctionType type = d->GetType();

        int elementsCounter;

        buffer << type << " ";
        switch (type)
        {
        case Rect:
            buffer << d->GetAnchor().x << " "
                   << d->GetAnchor().y << " "
                   << d->GetWidth() << " "
                   << d->GetHeight() << " ";
            break;
        case Circle:
            buffer << d->GetAnchor().x << " "
                   << d->GetAnchor().y << " "
                   << d->GetWidth() / 2 << " ";
            break;
        case Triangle:
            buffer << d->GetAnchor().x << " "
                   << d->GetAnchor().y << " "
                   << d->GetWidth() << " "
                   << d->GetHeight() << " ";
            break;
        case Poly:
            elementsCounter = d->GetElementsCount();
            buffer << elementsCounter << " ";

            for (int i = 0; i < elementsCounter; i++)
            {
                buffer << d->GetOriginPoints()[i].x + d->GetAnchor().x << " "
                       << d->GetOriginPoints()[i].y + d->GetAnchor().y << " ";
            }
            break;
        default:
            exit(1);
            break;
        }

        Color color = d->GetColor();
        bool fillFlag = d->GetFillFlag();
        float angle = d->GetAngle();
        Vector2 proportion = d->GetProportion();

        buffer << color.r << " " << color.g << " " << color.b << " " << fillFlag << " "
               << angle << " " << proportion.x << " " << proportion.y << std::endl;
    }

    std::ofstream file("./figuras.gr");

    if (!file)
    {
        return;
    }

    string encrypted = Encrypt(buffer.str(), EncryptionKey);
    file << encrypted;

    file.close();
}

// Lê no arquivo os atributos e reconstrói os desenhos,
// adicionando-os na lista.
void LoadFromFile(vector<Drawing*>&drawings)
{
    std::ifstream file("./figuras.gr");
    std::stringstream buffer, temp;

    if (!file.is_open())
    {
        return;
    }

    if (file.peek() == EOF)
    {
        return;
    }

    temp << file.rdbuf();
    string decrypted = Decrypt(temp.str(), EncryptionKey);
    buffer << decrypted;

    file.close();

    int numberOfDrawings;
    buffer >> numberOfDrawings;

    Drawing* drawing;
    FunctionType type;

    int typeIndex;
    float x1, y1, width, height, radius;
    int elementsCounter;

    for (int i = 0; i < numberOfDrawings; i++)
    {
        buffer >> typeIndex;

        type = static_cast<FunctionType>(typeIndex);

        switch (type)
        {
        case Rect:
            buffer >> x1 >> y1 >> width >> height;
            drawing = new RectangleDrawing(x1, y1, x1 + width, y1 + height);
            break;
        case Triangle:
            buffer >> x1 >> y1 >> width >> height;
            drawing = new TriangleDrawing(x1, y1, width, height);
            break;
        case Circle:
            buffer >> x1 >> y1 >> radius;
            drawing = new CircleDrawing(x1, y1, radius, 32);
            break;
        case Poly:
        {
            buffer >> elementsCounter;

            float* xs = new float[elementsCounter];
            float* ys = new float[elementsCounter];

            for (int i = 0; i < elementsCounter; i++)
            {
                buffer >> xs[i] >> ys[i];
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

        buffer >> r >> g >> b >> fillFlag >> angle >> proportionX >> proportionY;
        drawing->SetColor(Color(r, g, b));
        drawing->SetFillFlag(fillFlag);
        drawing->LoadProportion(Vector2(proportionX, proportionY));
        drawing->ApplyAngle(angle);

        drawings.push_back(drawing);
    }
}

#endif // FILEHANDLER_H_INCLUDED
