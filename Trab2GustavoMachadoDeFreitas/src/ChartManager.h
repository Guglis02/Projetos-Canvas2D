#ifndef CHARTMANAGER_H_INCLUDED
#define CHARTMANAGER_H_INCLUDED

#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <vector>
#include <string>

#include "gl_canvas2d.h"
#include "Chart.h"
#include "Transformations.h"
#include "FileHandler.h"

/** \brief
 * Classe responsável por controlar os gráficos do programa.
 */
class ChartManager
{
public:
    ChartManager(int screenWidth, int screenHeight)
    {
        StartCharts(screenWidth, screenHeight);
    }

    // Atualiza dimensões dos gráficos.
    void Update(int screenWidth, int screenHeight)
    {
        for (Chart* chart : charts)
        {
            chart->Update(screenWidth * 0.4, screenHeight * 0.3);
            chart->Render();
        }
    }

    // Callback do slider de amostras, determina a quantidade
    void SetNumberOfValues(float n)
    {
        this->numberOfValues = n;
    }

    // Callback do slider de quantitização, atualiza em tempo real os gráficos
    void SetQuantitizationCoefficient(float n)
    {
        this->quantitizationCoef = n;

        if (!input.empty())
        {
            RefreshCharts();
        }
    }

    // Callbacks usadas nos geradores de seno, step e sawtooth
    void SetValuesFreq(float n)
    {
        this->valuesFreq = n;
    }
    void SetValuesAmp(float n)
    {
        this->valuesAmp = n;
    }

    // Gera n valores aleatórios no range -128 a 127
    void GenerateRandomInput()
    {
        input.clear();
        srand(time(NULL));

        for (int i = 0; i < numberOfValues; i++)
        {
            input.push_back(rand() % 256 - 128);
        }

        RefreshCharts();
    }

    // Gera n valores em uma progressão senoide.
    void GenerateSineInput()
    {
        input.clear();

        for (int i = 0; i < numberOfValues; i++)
        {
            double step = i / (double)numberOfValues;
            input.push_back(valuesAmp * sin(PI_2 * step * valuesFreq));
        }

        RefreshCharts();
    }

    // Gera n valores de uma periodic step function
    void GenerateStepInput()
    {
        input.clear();
        int counter = 0;

        for (int i = 0; i < numberOfValues; i++)
        {
            if (counter < valuesFreq / 2)
            {
                input.push_back(valuesAmp);
            }
            else
            {
                input.push_back(-valuesAmp);
            }
            counter += 1 / (double)numberOfValues;
            if (counter >= valuesFreq)
                counter = 0;
        }

        RefreshCharts();
    }

    // Gera n valores seguindo uma progressão dente de serra.
    void GenerateSawtoothInput(void)
    {
        input.clear();
        double step = 1 / (valuesFreq * numberOfValues);

        for (int i = 0; i < numberOfValues; i++)
        {
            input.push_back(fmod(i * step, 1.0) * valuesAmp);
        }

        RefreshCharts();
    }

    void SaveInput()
    {
        SaveInFile(input, "./Trab2GustavoMachadoDeFreitas/input.dct");
    }

    void SaveOutput()
    {
        SaveInFile(idct, "./Trab2GustavoMachadoDeFreitas/output.dct");
    }

    void LoadInput()
    {
        LoadFromFile(input);
        RefreshCharts();
    }

private:
    int numberOfValues;
    float quantitizationCoef;
    float valuesFreq;
    float valuesAmp;

    Chart* inputChart;
    Chart* dctChart;
    Chart* idctChart;
    Chart* diffChart;

    std::vector<Chart*> charts;
    std::vector<double> input;
    std::vector<double> dct;
    std::vector<double> idct;
    std::vector<double> diff;

    // Inicializa os 4 gráficos em suas respectivas posições
    void StartCharts(int screenWidth, int screenHeight)
    {
        int xPadding = 100;
        int yPadding = 150;
        int ySpacing = 100;
        float qSW = screenWidth * 0.4; // a quarter screenWidth
        float tSH = screenHeight * 0.3; // a third screenHeight
        inputChart = new Chart(xPadding, yPadding + ySpacing + tSH, qSW, tSH, "Input");
        dctChart = new Chart(xPadding, yPadding, qSW, tSH, "DCT");
        idctChart = new Chart(2 * xPadding + qSW, yPadding + ySpacing + tSH, qSW, tSH, "IDCT");
        diffChart = new Chart(2 * xPadding + qSW, yPadding, qSW, tSH, "Diff");

        charts.push_back(inputChart);
        charts.push_back(idctChart);
        charts.push_back(dctChart);
        charts.push_back(diffChart);
    }

    // Atualiza os valores dos gráficos
    void RefreshCharts()
    {
        this->inputChart->CreatePoints(input);
        this->dct = Transformations::DiscreteCosineTransform(input, quantitizationCoef);
        this->dctChart->CreatePoints(dct);
        this->idct = Transformations::InverseDiscreteCosineTransform(dct);
        this->idctChart->CreatePoints(idct);
        this->diff = Transformations::CalculateDiff(input, idct);
        this->diffChart->CreatePoints(diff);
    }
};

#endif // CHARTMANAGER_H_INCLUDED
