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

class ChartManager
{
public:
    ChartManager(int screenWidth, int screenHeight, int valuesToGenerate)
    {
        this->valuesToGenerate = valuesToGenerate;
        StartCharts(screenWidth, screenHeight);
    }

    void Update(int screenWidth, int screenHeight)
    {
        for (Chart* chart : charts)
        {
            chart->Update(screenWidth * 0.4, screenHeight * 0.3);
            chart->Render();
        }
    }

    void GenerateRandomInput()
    {
        input.clear();
        srand(time(NULL));

        for (int i = 0; i < valuesToGenerate; i++)
        {
            input.push_back(rand() % 256 - 128);
        }

        RefreshCharts();
    }

    void GenerateSineInput()
    {
        input.clear();
        double phaseIncrement = 2.0 * PI / static_cast<double>(valuesToGenerate);

        for (int i = 0; i < valuesToGenerate; i++)
        {
            input.push_back(100 * sin(i * phaseIncrement));
        }

        RefreshCharts();
    }

    void GenerateStepInput()
    {
        input.clear();
        int stepSize = valuesToGenerate / 4;

        for (int i = 0; i < valuesToGenerate; i++)
        {
            if (i % stepSize == 0)
            {
                input.push_back(100);
            }
            else
            {
                input.push_back(-100);
            }
        }

        RefreshCharts();
    }

    void GenerateSawtoothInput()
    {
        input.clear();

        for (int i = 0; i < valuesToGenerate; i++)
        {
            input.push_back((i % 100) - 50);
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
        printf("\ncarrega");
        LoadFromFile(input);
        RefreshCharts();
    }

private:
    int valuesToGenerate;

    Chart* inputChart;
    Chart* dctChart;
    Chart* idctChart;
    Chart* diffChart;

    std::vector<Chart*> charts;
    std::vector<double> input;
    std::vector<double> dct;
    std::vector<double> idct;
    std::vector<double> diff;

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

    void RefreshCharts()
    {
        this->inputChart->CreatePoints(input);
        this->dct = Transformations::DiscreteCosineTransform(input, 1.0);
        this->dctChart->CreatePoints(dct);
        this->idct = Transformations::InverseDiscreteCosineTransform(dct);
        this->idctChart->CreatePoints(idct);
        this->diff = Transformations::CalculateDiff(input, idct);
        this->diffChart->CreatePoints(diff);
    }
};


#endif // CHARTMANAGER_H_INCLUDED
