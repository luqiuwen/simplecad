
//基础函数库：弧 实现好
//=========================

//-------------------------------------------------------------------------------

#include "circle.h"
#include "arc.h"
#include "window.h"
#include "calc.h"

#include <stdio.h>

//构造函数
void
graph_arc_init(struct graph_arc * cthis,struct point center,int radius,float angle_start,float angle_end,float angle)
{
    graph_init(&(cthis->graph)); //调用父类的构造函数
    
    //覆写父类的虚函数
    (cthis->graph).draw       = graph_arc_draw;
    (cthis->graph).is_cross   = graph_arc_is_cross;
    (cthis->graph).move       = graph_arc_move;
    (cthis->graph).rot        = graph_arc_rot;
    (cthis->graph).resize     = graph_arc_resize;
    (cthis->graph).get_center = graph_arc_get_center;
    (cthis->graph).deepcopy   = graph_arc_deepcopy;
    (cthis->graph).draw_tag   = graph_arc_draw_tag;
    (cthis->graph).get_tag    = graph_arc_get_tag;
    
    //构建子类的函数指针
    cthis->insert          = graph_arc_insert;
    cthis->del             = graph_arc_del;
    cthis->get_child       = graph_arc_get_child;
    cthis->get_parent      = graph_arc_get_parent;
    cthis->get_child_count = graph_arc_get_child_count;
    
    //数据初始化
    cthis->center          = center;
    cthis->radius          = radius;
    cthis->angle_start     = angle_start;
    cthis->angle_end       = angle_end;
    cthis->angle           = angle;
    return;
}

//析构函数
void graph_arc_exit(struct graph_arc * cthis)
{
    return;
}

//获得新对象
struct graph_arc * graph_arc_new(struct point center,int radius,float angle_start,float angle_end,float angle)
{
    struct graph_arc * cthis = malloc(sizeof(struct graph_arc));

    if(!cthis)
        return NULL;
    graph_arc_init(cthis,center,radius,angle_start,angle_end,angle);
    return cthis;
}
//删除对象
void graph_arc_delete(struct graph_arc * cthis)
{
    free(cthis);
    return;
}
 
//-------------------------------------------------------------------------------
//继承自graph的方法实现：调用父对象函数

int graph_arc_insert(struct graph_arc *cthis,struct graph_arc *item)
{
    return graph_insert((struct graph *)cthis,(struct graph *)item);
}
//删除子元
int graph_arc_del(struct graph_arc *cthis,int item_id)
{
    return graph_del((struct graph *)cthis,item_id);
}
//返回子元序列
struct graph** graph_arc_get_child(struct graph_arc *cthis)
{
    return graph_get_child((struct graph *)cthis);
}
//返回父元
struct graph* graph_arc_get_parent(struct graph_arc *cthis)
{
    return graph_get_parent((struct graph *)cthis);
}
//返回子代数量
int graph_arc_get_child_count(struct graph_arc *cthis)
{
    return graph_get_child_count((struct graph *)cthis);
}

//-------------------------------------------------------------------------------
//实现的虚函数

int graph_arc_draw(struct graph * cthis, struct window * draw_object)
{
    //从接口恢复对象
    struct graph_arc *cthis_tf = (struct graph_arc *)cthis;

    draw_object->draw_eclipse(draw_object,cthis_tf->center,cthis_tf->radius,
                              cthis_tf->radius,cthis_tf->angle,cthis_tf->angle_start,cthis_tf->angle_end);

    return 0;
}

//TODO:
int graph_arc_is_cross(struct graph *cthis, struct point point)
{
    struct graph_arc *cthis_tf = (struct graph_arc *)cthis;
    return 0;
}

int graph_arc_move(struct graph *cthis,struct point base,struct point vector)
{
    struct graph_arc *cthis_tf = (struct graph_arc *)cthis;

    //计算相对位移
    (cthis_tf->center).x += vector.x;
    (cthis_tf->center).y += vector.y;

    return 0;
}

int graph_arc_rot(struct graph *cthis,struct point base,float angle)
{
    struct graph_arc *cthis_tf = (struct graph_arc *)cthis;

    cthis_tf->center = calc_baserot(cthis_tf->center,base,angle);
    cthis_tf->angle  += (angle/3.1416) * 360; //弧度化角度

    return 0;
}

int graph_arc_resize(struct graph *cthis,struct point base,float rate)
{
    struct graph_circle *cthis_tf = (struct graph_circle *)cthis;

    int resize_radius = (int)(cthis_tf->radius * rate);
    cthis_tf->radius  = resize_radius;
    cthis_tf->center  = calc_scale(cthis_tf->center,base,rate);
    cthis_tf->center  = calc_scale(cthis_tf->center,base,rate);

    return 0;
}
struct point graph_arc_get_center(struct graph *cthis)
{
    struct graph_arc *cthis_tf = (struct graph_arc *)cthis;
    return cthis_tf->center;
}
struct graph * graph_arc_deepcopy(struct graph *cthis)
{
    struct graph_arc *cthis_tf = (struct graph_arc *)cthis;

    return (struct graph *)graph_arc_new(cthis_tf->center,
                                         cthis_tf->radius,
                                         cthis_tf->angle_start,
                                         cthis_tf->angle_end,
                                         cthis_tf->angle);
}

//画图元标志
int graph_arc_draw_tag(struct graph *cthis,struct window * draw_object)
{
    struct graph_arc *cthis_tf = (struct graph_arc *)cthis;

    char tags[100];
    sprintf(tags,"A(%d)",cthis->object_id);

    window_draw_tag(draw_object,cthis_tf->center,tags);

    return 0;
}

int graph_arc_get_tag(struct graph *cthis, struct graph_description *out_info, char *out_info_str)
{
    return 0;
}