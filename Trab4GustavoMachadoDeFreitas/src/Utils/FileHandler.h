#ifndef FILEHANDLER_H_INCLUDED
#define FILEHANDLER_H_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Arquivo utilitário contendo funções relacionadas a lidar com arquivos.
// Possui métodos auxiliares que usam cifra de ceasar para criptografar o arquivo.

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

void SaveInFile(int highScore)
{
    std::stringstream buffer;

    buffer << highScore << std::endl;

    std::ofstream file("./Trab3GustavoMachadoDeFreitas/highScore.gr");

    if (!file)
    {
        return;
    }

    string encrypted = Encrypt(buffer.str(), EncryptionKey);
    file << encrypted;

    file.close();
}

void LoadFromFile(int &highScore)
{
    std::ifstream file("./Trab3GustavoMachadoDeFreitas/highScore.gr");
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

    buffer >> highScore;
}

#endif // FILEHANDLER_H_INCLUDED
