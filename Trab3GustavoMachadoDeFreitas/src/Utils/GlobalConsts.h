#ifndef GLOBALCONSTS_H_INCLUDED
#define GLOBALCONSTS_H_INCLUDED

int const ConstScreenWidth = 800;
int const ConstScreenHeight = 736;

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

#endif // GLOBALCONSTS_H_INCLUDED
