
//基础函数库：直线   实现
//=========================

//-------------------------------------------------------------------------------

#include <stdio.h>
#include <math.h>

#include "line.h"
#include "graph.h"
#include "window.h"
#include "calc.h"

static int __graph_line_calc_center(struct graph_line *cthis)
//维护中心点信息
{
    return 0;
}

//构造函数
void graph_line_init(struct graph_line * cthis,struct point pstart,struct point pend)
{
    graph_init(&(cthis->graph)); //调用父类的构造函数
    
    //覆写父类的虚函数
    (cthis->graph).draw       = graph_line_draw;
    (cthis->graph).is_cross   = graph_line_is_cross;
    (cthis->graph).move       = graph_line_move;
    (cthis->graph).rot        = graph_line_rot;
    (cthis->graph).resize     = graph_line_resize;
    (cthis->graph).get_center = graph_line_get_center;
    (cthis->graph).deepcopy   = graph_line_deepcopy;
    (cthis->graph).draw_tag   = graph_line_draw_tag;
    (cthis->graph).get_tag    = graph_line_get_tag;

    //构建子类的函数指针
    cthis->insert           = graph_line_insert;
    cthis->del              = graph_line_del;
    cthis->get_child        = graph_line_get_child;
    cthis->get_parent       = graph_line_get_parent;
    cthis->get_child_count  = graph_line_get_child_count;

    //数据初始化
    cthis->pstart = pstart;
    cthis->pend   = pend;

    return;
}

//析构函数
void graph_line_exit(struct graph_line * cthis)
{
    return;
}

//获得新对象
struct graph_line * graph_line_new(struct point pstart,struct point pend)
{
    struct graph_line * cthis = malloc(sizeof(struct graph_line));

    if(!cthis)
        return NULL;
    graph_line_init(cthis,pstart,pend);
    return cthis;
}
//删除对象
void graph_line_delete(struct graph_line * cthis)
{
    free(cthis);
    return;
}
 
//-------------------------------------------------------------------------------
//继承自graph的方法实现：调用父对象函数

int graph_line_insert(struct graph_line *cthis,struct graph_line *item)
{
    return graph_insert((struct graph *)cthis,(struct graph *)item);
}
//删除子元
int graph_line_del(struct graph_line *cthis,int item_id)
{
    return graph_del((struct graph *)cthis,item_id);
}
//返回子元序列
struct graph** graph_line_get_child(struct graph_line *cthis)
{
    return graph_get_child((struct graph *)cthis);
}
//返回父元
struct graph* graph_line_get_parent(struct graph_line *cthis)
{
    return graph_get_parent((struct graph *)cthis);
}
//返回子代数量
int graph_line_get_child_count(struct graph_line *cthis)
{
    return graph_get_child_count((struct graph *)cthis);
}

//-------------------------------------------------------------------------------
//实现的虚函数

int graph_line_draw(struct graph * cthis, struct window * draw_object)
{
    //从接口恢复对象
    struct graph_line *cthis_tf = (struct graph_line *)cthis;
    //画线
    draw_object->draw_line(draw_object,cthis_tf->pstart,cthis_tf->pend);

    return 0;
}

//TODO:
int graph_line_is_cross(struct graph *cthis, struct point point)
{
    struct graph_line *cthis_tf = (struct graph_line *)cthis;
    return 0;
}

int graph_line_move(struct graph *cthis,struct point base,struct point vector)
{
    struct graph_line *cthis_tf = (struct graph_line *)cthis;

    //计算相对位移
    (cthis_tf->pstart).x += vector.x;
    (cthis_tf->pend).x   += vector.x;
    (cthis_tf->pstart).y += vector.y;
    (cthis_tf->pend).y   += vector.y;
    
    return 0;
}

int graph_line_rot(struct graph *cthis,struct point base,float angle)
{
    struct graph_line *cthis_tf = (struct graph_line *)cthis;

    (cthis_tf->pstart).x 
    = (int)(base.x + ((cthis_tf->pstart).x - base.x)*cos(angle) - ((cthis_tf->pstart).y - base.y ) * sin(angle));

    (cthis_tf->pstart).y
    = (int)(base.y + ((cthis_tf->pstart).x - base.x)*sin(angle) - ((cthis_tf->pstart).y - base.y ) * cos(angle));

    (cthis_tf->pend).x
    = (int)(base.x + ((cthis_tf->pend).x - base.x)*cos(angle) - ((cthis_tf->pend).y - base.y ) * sin(angle));

    (cthis_tf->pend).y
    = (int)(base.y + ((cthis_tf->pend).x - base.x)*sin(angle) - ((cthis_tf->pend).y - base.y ) * cos(angle));

    return 0;
}

int graph_line_resize(struct graph *cthis,struct point base,float rate)
{
    struct graph_line *cthis_tf = (struct graph_line *)cthis;

    cthis_tf->pstart = calc_scale(cthis_tf->pstart,base,rate);
    cthis_tf->pend   = calc_scale(cthis_tf->pend,base,rate);

    return 0;
}
struct point graph_line_get_center(struct graph *cthis)
{
    struct graph_line *cthis_tf = (struct graph_line *)cthis;

    return calc_center(cthis_tf->pstart,cthis_tf->pend);
}
struct graph *graph_line_deepcopy(struct graph *cthis)
{
    struct graph_line *cthis_tf = (struct graph_line *)cthis;

    return (struct graph *)graph_line_new(cthis_tf->pstart,cthis_tf->pend);
}

int graph_line_draw_tag(struct graph *cthis,struct window * draw_object)
{
    struct graph_line *cthis_tf = (struct graph_line *)cthis;
    char tags[100];
    sprintf(tags,"L(%d)",cthis->object_id);

    window_draw_tag(draw_object,cthis_tf->pstart,tags);

    return 0;
}
int graph_line_get_tag(struct graph *cthis, struct graph_description *out_info, char *out_info_str)
{
    return 0;
}