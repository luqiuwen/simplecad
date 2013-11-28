
//基础函数库：直线   实现
//=========================

//-------------------------------------------------------------------------------

#include "circle.h"
#include "graph.h"
#include "window.h"
#include "calc.h"

#include <stdio.h>

//构造函数
void graph_circle_init(struct graph_circle * cthis,struct point center,int radius)
{
    graph_init(&(cthis->graph)); //调用父类的构造函数
    
    //覆写父类的虚函数
    (cthis->graph).draw       = graph_circle_draw;
    (cthis->graph).is_cross   = graph_circle_is_cross;
    (cthis->graph).move       = graph_circle_move;
    (cthis->graph).rot        = graph_circle_rot;
    (cthis->graph).resize     = graph_circle_resize;
    (cthis->graph).get_center = graph_circle_get_center;
    (cthis->graph).deepcopy   = graph_circle_deepcopy;
    (cthis->graph).draw_tag   = graph_circle_draw_tag;
    (cthis->graph).get_tag    = graph_circle_get_tag;
    
    //构建子类的函数指针
    cthis->insert           = graph_circle_insert;
    cthis->del              = graph_circle_del;
    cthis->get_child        = graph_circle_get_child;
    cthis->get_parent       = graph_circle_get_parent;
    cthis->get_child_count  = graph_circle_get_child_count;

    //数据初始化
    cthis->center           = center;
    cthis->radius           = radius;

    return;
}

//析构函数
void graph_circle_exit(struct graph_circle * cthis)
{
    return;
}

//获得新对象
struct graph_circle * graph_circle_new(struct point center,int radius)
{
    struct graph_circle * cthis = malloc(sizeof(struct graph_circle));

    if(!cthis)
        return NULL;
    graph_circle_init(cthis,center,radius);
    return cthis;
}
//删除对象
void graph_circle_delete(struct graph_circle * cthis)
{
    free(cthis);
    return;
}
 
//-------------------------------------------------------------------------------
//继承自graph的方法实现：调用父对象函数

int graph_circle_insert(struct graph_circle *cthis,struct graph_circle *item)
{
    return graph_insert((struct graph *)cthis,(struct graph *)item);
}
//删除子元
int graph_circle_del(struct graph_circle *cthis,int item_id)
{
    return graph_del((struct graph *)cthis,item_id);
}
//返回子元序列
struct graph** graph_circle_get_child(struct graph_circle *cthis)
{
    return graph_get_child((struct graph *)cthis);
}
//返回父元
struct graph* graph_circle_get_parent(struct graph_circle *cthis)
{
    return graph_get_parent((struct graph *)cthis);
}
//返回子代数量
int graph_circle_get_child_count(struct graph_circle *cthis)
{
    return graph_get_child_count((struct graph *)cthis);
}

//-------------------------------------------------------------------------------
//实现的虚函数

int graph_circle_draw(struct graph * cthis, struct window * draw_object)
{
    //从接口恢复对象
    struct graph_circle *cthis_tf = (struct graph_circle *)cthis;

    draw_object->draw_circle(draw_object,cthis_tf->center,cthis_tf->radius);

    return 0;
}

//TODO:
int graph_circle_is_cross(struct graph *cthis, struct point point)
{
    struct graph_circle *cthis_tf = (struct graph_circle *)cthis;
    return 0;
}

int graph_circle_move(struct graph *cthis,struct point base,struct point vector)
{
    struct graph_circle *cthis_tf = (struct graph_circle *)cthis;

    //计算相对位移
    (cthis_tf->center).x += vector.x;
    (cthis_tf->center).y += vector.y;

    return 0;
}

int graph_circle_rot(struct graph *cthis,struct point base,float angle)
{
    struct graph_circle *cthis_tf = (struct graph_circle *)cthis;

    cthis_tf->center = calc_baserot(cthis_tf->center,base,angle);

    return 0;
}

int graph_circle_resize(struct graph *cthis,struct point base,float rate)
{
    struct graph_circle *cthis_tf = (struct graph_circle *)cthis;

    int resize_radius = (int)(cthis_tf->radius * rate);
    cthis_tf->radius  = resize_radius;
    cthis_tf->center  = calc_scale(cthis_tf->center,base,rate);

    return 0;
}
struct point graph_circle_get_center(struct graph *cthis)
{
    struct graph_circle *cthis_tf = (struct graph_circle *)cthis;
    return cthis_tf->center;
}
struct graph * graph_circle_deepcopy(struct graph *cthis)
{
    struct graph_circle *cthis_tf = (struct graph_circle *)cthis;

    return (struct graph *)graph_circle_new(cthis_tf->center,cthis_tf->radius);
}

//画图元标志
int graph_circle_draw_tag(struct graph *cthis,struct window * draw_object)
{
    struct graph_circle *cthis_tf = (struct graph_circle *)cthis;

    char tags[100];
    sprintf(tags,"C(%d)",cthis->object_id);

    window_draw_tag(draw_object,cthis_tf->center,tags);

    return 0;
}

int graph_circle_get_tag(struct graph *cthis, struct graph_description *out_info, char *out_info_str)
{
    return 0;
}