
//公共计算函数

#include "point.h"

struct point calc_baserot(struct point point,struct point base,float angle);
struct point calc_center(struct point p1,struct point p2);
struct point calc_scale(struct point point,struct point base,float rate);