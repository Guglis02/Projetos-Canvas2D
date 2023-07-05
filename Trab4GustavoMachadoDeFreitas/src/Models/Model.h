#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "../Utils/VectorHomo3d.h"
#include "../Utils/Matrix3d.h"

using namespace std;

class Model
{
public:
    Model(VectorHomo3d center, int steps)
    {
        this->center = center;
        this->steps = steps;
        this->points.resize(steps);
        for (int i = 0; i < steps; i++)
        {
            this->points[i].resize(steps);
        }
        transformationMatrix = new Matrix3d();
    }
    ~Model()
    {
        delete transformationMatrix;
    }

    VectorHomo3d GetCenter()
    {
        return center;
    }

    void LocalRotate(float anglex, float angley, float anglez, bool isPerm)
    {
        LocalRotate(anglex, angley, anglez, isPerm, center);
    } 

    void LocalRotate(float anglex, float angley, float anglez, bool isPerm, VectorHomo3d anchor)
    {
        transformationMatrix->Reset();
        transformationMatrix->Translation(anchor);
        transformationMatrix->RotationX(anglex);
        transformationMatrix->RotationY(angley);
        transformationMatrix->RotationZ(anglez);
        transformationMatrix->Translation(anchor * -1);
            
        transformedPoints = transformationMatrix->ApplyToPoints(transformedPoints);

        if (isPerm)
        {
            points = transformationMatrix->ApplyToPoints(points);
        }
    }

    void Reposition(VectorHomo3d newCenter, bool isPerm)
    {
        transformationMatrix->Reset();
        transformationMatrix->Translation(center * -1);
        center = newCenter;
        transformationMatrix->Translation(newCenter);

        transformedPoints = transformationMatrix->ApplyToPoints(points);

        if (isPerm)
        {
            points = transformationMatrix->ApplyToPoints(points);
        }
    }

    void ResetTransformedPoints()
    {
        transformedPoints = points;
    }

    void GlobalRotate(int anglex, int angley, int anglez, VectorHomo3d anchor)
    {
        transformationMatrix->Reset();
        transformationMatrix->Translation(anchor);
        transformationMatrix->RotationX(DegToRad(anglex));
        transformationMatrix->RotationY(DegToRad(angley));
        transformationMatrix->RotationZ(DegToRad(anglez));
        transformationMatrix->Translation(anchor * -1);
        
        transformedPoints = transformationMatrix->ApplyToPoints(transformedPoints);
    }

    vector<vector<VectorHomo3d>> points;

    virtual void Build(){};

    void DrawPerspective(int d)
    {
        for (unsigned int i = 0; i < transformedPoints.size(); i++)
        {
            for (unsigned int j = 0; j < transformedPoints[i].size(); j++)
            {
                int x = i;
                int y = j;
                int nx = (i + 1) % (transformedPoints.size());
                int ny = (j + 1) % (transformedPoints[i].size());
                CV::perspectiveLine(transformedPoints[x][y], transformedPoints[x][nx], d);
                CV::perspectiveLine(transformedPoints[x][y], transformedPoints[nx][y], d);
                CV::perspectiveLine(transformedPoints[x][y], transformedPoints[nx][ny], d);
            }
        }
    }

    void DrawOrthogonal()
    {
        for (int i = 0; i < transformedPoints.size(); i++)
        {
            for (int j = 0; j < transformedPoints[i].size(); j++)
            {
                int x = i;
                int y = j;
                int nx = (i + 1) % (transformedPoints.size());
                int ny = (j + 1) % (transformedPoints[i].size());
                CV::line(transformedPoints[x][y], transformedPoints[x][nx]);
                CV::line(transformedPoints[x][y], transformedPoints[nx][y]);
                CV::line(transformedPoints[x][y], transformedPoints[nx][ny]);
            }
        }
    }
protected:
    Matrix3d* transformationMatrix;
    vector<vector<VectorHomo3d>> transformedPoints;
    VectorHomo3d center;
    int steps = 20;
};

#endif
