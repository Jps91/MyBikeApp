#include "Interpolation.h"

Interpolation::Interpolation()
{
    ;
}

void Interpolation::linearSet(double x0, double y0, double x1, double y1)
{
    linearCacheX0 = x0;
    linearCacheX1 = x1;
    linearCacheY0 = y0;
    linearCacheY1 = y1;
    linearCahceDeltaX = (x1 - x0);
    linearCahceDeltaY = (y1 - y0);
}

double Interpolation::linear(double x0, double y0, double x1, double y1, double x)
{
    if (x0 == x1) 
    {
        return 0.0;
    }
    double y = y0 + ((x - x0) / (x1 - x0)) * (y1 - y0);

    return y;
}

double Interpolation::linearSameIntervall(double x) //becarefull x should never be the same as before!!
{
    return linearCacheY0 + ((x - linearCacheX0) / linearCahceDeltaX) * (linearCahceDeltaY);
}

double Interpolation::linearNext(double nextX1, double nextY1, double x)
{
    linearCacheX0 = linearCacheX1;
    linearCacheX1 = nextX1;
    linearCacheY0 = linearCacheY1;
    linearCacheY0 = nextY1;
    linearCahceDeltaX = (linearCacheX1 - linearCacheX0);
    linearCahceDeltaY = (linearCacheY1 - linearCacheY0);
    
    return linearCacheY0 + ((x - linearCacheX0) / linearCahceDeltaX) * (linearCahceDeltaY);
}
