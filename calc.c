
//公共计算函数

#include <math.h>
#include "point.h"

struct point calc_baserot(struct point point,struct point base,float angle)
{
    //二维平面旋转
    struct point result;

    result.x 
    = (int)(base.x + (point.x - base.x)*cos(angle) - (point.y - base.y ) * sin(angle));

    result.y
    = (int)(base.y + (point.x - base.x)*sin(angle) + (point.y - base.y ) * cos(angle));

    return result;
}

struct point calc_center(struct point p1,struct point p2)
{
    //计算中点
    return POINT((p1.x+p2.x)/2,(p1.y+p2.y)/2);
}

struct point calc_scale(struct point point,struct point base,float rate)
{
    //计算缩放
    struct point result;
    result.x = (int)(point.x * rate + base.x*(1-rate));
    result.y = (int)(point.y * rate + base.y*(1-rate));

    return result;
}