#ifndef GLOBALCONSTS_H_INCLUDED
#define GLOBALCONSTS_H_INCLUDED

#define PI_2 6.28318530717
#define PI   3.14159265359

// Arquivo com constantes globais

// Tamanhos da tela
int const ConstScreenWidth = 800;
int const ConstScreenHeight = 736;

// Métodos utilitários envolvendo o tamanho da tela
static bool IsOutOfBounds(VectorHomo point)
{
    return point.x < 0 || point.x > ConstScreenWidth || point.y < 0 || point.y > ConstScreenHeight;
}

static bool IsOutOfBounds(VectorHomo point, float tolerance)
{
    return point.x < 0 - tolerance || point.x > ConstScreenWidth + tolerance || point.y < 0 - tolerance || point.y > ConstScreenHeight + tolerance;
}

static bool IsOutOfBounds(vector<VectorHomo> point)
{
    for (auto p : point)
    {
        if (IsOutOfBounds(p))
        {
            return true;
        }
    }

    return false;
}

// Tamanho da fonte usada no canvas
int const ApproximateFontSize = 10;

// Quantos pontos o jogador precisa para vencer o modo trench run
int const TargetPoints = 200;

#endif // GLOBALCONSTS_H_INCLUDED