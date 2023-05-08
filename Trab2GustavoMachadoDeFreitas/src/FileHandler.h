#ifndef FILEHANDLER_H_INCLUDED
#define FILEHANDLER_H_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

/** \brief
 * Arquivo utilitário contendo funções relacionadas a lidar com arquivos.
 */

void SaveInFile(vector<double> values, const char* filePath)
{
    std::stringstream buffer;

    unsigned int numberOfValues = values.size();

    buffer << numberOfValues << std::endl;

    for (double value : values)
    {
        char charValue = static_cast<char>(value);
        buffer << charValue << std::endl;
    }

    std::ofstream file(filePath);

    if (!file)
    {
        return;
    }

    file << buffer.str();

    file.close();
}

void LoadFromFile(vector<double>& values)
{
    std::ifstream file("./Trab2GustavoMachadoDeFreitas/input.dct");

    if (!file.is_open())
    {
        return;
    }

    if (file.peek() == EOF)
    {
        return;
    }

    unsigned int numberOfValues;
    file >> numberOfValues;

    values.clear();
    for (int i = 0; i < numberOfValues; i++)
    {
        char charValue;
        if (!(file >> charValue))
        {
            // Nâo conseguiu ler valor no arquivo.
            file.close();
            return;
        }

        double value = static_cast<double>(charValue);;

        values.push_back(value);
    }

    file.close();
}
#endif // FILEHANDLER_H_INCLUDED
