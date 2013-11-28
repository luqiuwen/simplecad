
#include "point.h"

struct point POINT(int x,int y)
{
    struct point rt;
    rt.x = x;
    rt.y = y;
    return rt;
}