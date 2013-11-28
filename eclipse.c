
//基础函数库：椭圆 实现
//=========================

//-------------------------------------------------------------------------------

#include "circle.h"
#include "eclipse.h"
#include "window.h"
#include "calc.h"

#include <stdio.h>

//构造函数
void graph_eclipse_init(struct graph_eclipse * cthis,struct point center,int axes_x,int axes_y,float angle)
{
    graph_init(&(cthis->graph)); //调用父类的构造函数
    
    //覆写父类的虚函数
    (cthis->graph).draw       = graph_eclipse_draw;
    (cthis->graph).is_cross   = graph_eclipse_is_cross;
    (cthis->graph).move       = graph_eclipse_move;
    (cthis->graph).rot        = graph_eclipse_rot;
    (cthis->graph).resize     = graph_eclipse_resize;
    (cthis->graph).get_center = graph_eclipse_get_center;
    (cthis->graph).deepcopy   = graph_eclipse_deepcopy;
    (cthis->graph).draw_tag   = graph_eclipse_draw_tag;
    (cthis->graph).get_tag    = graph_eclipse_get_tag;
    
    //构建子类的函数指针
    cthis->insert          = graph_eclipse_insert;
    cthis->del             = graph_eclipse_del;
    cthis->get_child       = graph_eclipse_get_child;
    cthis->get_parent      = graph_eclipse_get_parent;
    cthis->get_child_count = graph_eclipse_get_child_count;
    
    //数据初始化
    cthis->center          = center;
    cthis->axes_x          = axes_x;
    cthis->axes_y          = axes_y;
    cthis->angle           = angle;
    return;
}

//析构函数
void graph_eclipse_exit(struct graph_eclipse * cthis)
{
    return;
}

//获得新对象
struct graph_eclipse * graph_eclipse_new(struct point center,int axes_x,int axes_y,float angle)
{
    struct graph_eclipse * cthis = malloc(sizeof(struct graph_eclipse));

    if(!cthis)
        return NULL;
    graph_eclipse_init(cthis,center,axes_x,axes_y,angle);
    return cthis;
}
//删除对象
void graph_eclipse_delete(struct graph_eclipse * cthis)
{
    free(cthis);
    return;
}
 
//-------------------------------------------------------------------------------
//继承自graph的方法实现：调用父对象函数

int graph_eclipse_insert(struct graph_eclipse *cthis,struct graph_eclipse *item)
{
    return graph_insert((struct graph *)cthis,(struct graph *)item);
}
//删除子元
int graph_eclipse_del(struct graph_eclipse *cthis,int item_id)
{
    return graph_del((struct graph *)cthis,item_id);
}
//返回子元序列
struct graph** graph_eclipse_get_child(struct graph_eclipse *cthis)
{
    return graph_get_child((struct graph *)cthis);
}
//返回父元
struct graph* graph_eclipse_get_parent(struct graph_eclipse *cthis)
{
    return graph_get_parent((struct graph *)cthis);
}
//返回子代数量
int graph_eclipse_get_child_count(struct graph_eclipse *cthis)
{
    return graph_get_child_count((struct graph *)cthis);
}

//-------------------------------------------------------------------------------
//实现的虚函数

int graph_eclipse_draw(struct graph * cthis, struct window * draw_object)
{
    //从接口恢复对象
    struct graph_eclipse *cthis_tf = (struct graph_eclipse *)cthis;

    draw_object->draw_eclipse(draw_object,cthis_tf->center,cthis_tf->axes_x,cthis_tf->axes_y,cthis_tf->angle,0,360);

    return 0;
}

//TODO:
int graph_eclipse_is_cross(struct graph *cthis, struct point point)
{
    struct graph_eclipse *cthis_tf = (struct graph_eclipse *)cthis;
    return 0;
}

int graph_eclipse_move(struct graph *cthis,struct point base,struct point vector)
{
    struct graph_eclipse *cthis_tf = (struct graph_eclipse *)cthis;

    //计算相对位移
    (cthis_tf->center).x += vector.x;
    (cthis_tf->center).y += vector.y;

    return 0;
}

int graph_eclipse_rot(struct graph *cthis,struct point base,float angle)
{
    struct graph_eclipse *cthis_tf = (struct graph_eclipse *)cthis;

    cthis_tf->center = calc_baserot(cthis_tf->center,base,angle);
    cthis_tf->angle  += (angle/3.1416) * 360; //弧度化角度

    return 0;
}

int graph_eclipse_resize(struct graph *cthis,struct point base,float rate)
{
    struct graph_eclipse *cthis_tf = (struct graph_eclipse *)cthis;

    int resize_axesx = (int)(cthis_tf->axes_x * rate);
    int resize_axesy = (int)(cthis_tf->axes_y * rate);
    cthis_tf->axes_x = resize_axesx;
    cthis_tf->axes_y = resize_axesy;
    
    cthis_tf->center  = calc_scale(cthis_tf->center,base,rate);

    return 0;
}
struct point graph_eclipse_get_center(struct graph *cthis)
{
    struct graph_eclipse *cthis_tf = (struct graph_eclipse *)cthis;
    return cthis_tf->center;
}
struct graph * graph_eclipse_deepcopy(struct graph *cthis)
{
    struct graph_eclipse *cthis_tf = (struct graph_eclipse *)cthis;
    return (struct graph *)graph_eclipse_new(cthis_tf->center,cthis_tf->axes_x,cthis_tf->axes_y,cthis_tf->angle);
}

//画图元标志
int graph_eclipse_draw_tag(struct graph *cthis,struct window * draw_object)
{
    struct graph_eclipse *cthis_tf = (struct graph_eclipse *)cthis;

    char tags[100];
    sprintf(tags,"E(%d)",cthis->object_id);

    window_draw_tag(draw_object,cthis_tf->center,tags);

    return 0;
}

int graph_eclipse_get_tag(struct graph *cthis, struct graph_description *out_info, char *out_info_str)
{
    return 0;
}