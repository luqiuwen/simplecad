
//基础函数库：直线   实现
//=========================

//-------------------------------------------------------------------------------

#include <stdio.h>
#include <math.h>


#include "rect.h"
#include "graph.h"
#include "window.h"
#include "calc.h"


static int __graph_rect_calc_center(struct graph_rect *cthis)
//维护中心点信息
{
    return 0;
}

//构造函数
void graph_rect_init(struct graph_rect * cthis,struct point pstart,struct point pend)
{
    graph_init(&(cthis->graph)); //调用父类的构造函数
    
    //覆写父类的虚函数
    (cthis->graph).draw       = graph_rect_draw;
    (cthis->graph).is_cross   = graph_rect_is_cross;
    (cthis->graph).move       = graph_rect_move;
    (cthis->graph).rot        = graph_rect_rot;
    (cthis->graph).resize     = graph_rect_resize;
    (cthis->graph).get_center = graph_rect_get_center;
    (cthis->graph).deepcopy   = graph_rect_deepcopy;
    (cthis->graph).draw_tag   = graph_rect_draw_tag;
    (cthis->graph).get_tag    = graph_rect_get_tag;

    //构建子类的函数指针
    cthis->insert           = graph_rect_insert;
    cthis->del              = graph_rect_del;
    cthis->get_child        = graph_rect_get_child;
    cthis->get_parent       = graph_rect_get_parent;
    cthis->get_child_count  = graph_rect_get_child_count;

    //数据初始化
    cthis->pstart = pstart;
    cthis->pend   = pend;
    cthis->pa = POINT((cthis->pend).x,(cthis->pstart).y);
    cthis->pb = POINT((cthis->pstart).x,(cthis->pend).y);

    return;
}

//析构函数
void graph_rect_exit(struct graph_rect * cthis)
{
    return;
}

//获得新对象
struct graph_rect * graph_rect_new(struct point pstart,struct point pend)
{
    struct graph_rect * cthis = malloc(sizeof(struct graph_rect));

    if(!cthis)
        return NULL;
    graph_rect_init(cthis,pstart,pend);
    return cthis;
}
//删除对象
void graph_rect_delete(struct graph_rect * cthis)
{
    free(cthis);
    return;
}
 
//-------------------------------------------------------------------------------
//继承自graph的方法实现：调用父对象函数

int graph_rect_insert(struct graph_rect *cthis,struct graph_rect *item)
{
    return graph_insert((struct graph *)cthis,(struct graph *)item);
}
//删除子元
int graph_rect_del(struct graph_rect *cthis,int item_id)
{
    return graph_del((struct graph *)cthis,item_id);
}
//返回子元序列
struct graph** graph_rect_get_child(struct graph_rect *cthis)
{
    return graph_get_child((struct graph *)cthis);
}
//返回父元
struct graph* graph_rect_get_parent(struct graph_rect *cthis)
{
    return graph_get_parent((struct graph *)cthis);
}
//返回子代数量
int graph_rect_get_child_count(struct graph_rect *cthis)
{
    return graph_get_child_count((struct graph *)cthis);
}

//-------------------------------------------------------------------------------
//实现的虚函数

int graph_rect_draw(struct graph * cthis, struct window * draw_object)
{
    //从接口恢复对象
    struct graph_rect *cthis_tf = (struct graph_rect *)cthis;

    //画四边
    window_draw_line(draw_object,cthis_tf->pstart,cthis_tf->pa);
    window_draw_line(draw_object,cthis_tf->pa,cthis_tf->pend);
    window_draw_line(draw_object,cthis_tf->pend,cthis_tf->pb);
    window_draw_line(draw_object,cthis_tf->pb,cthis_tf->pstart);

    return 0;
}

//TODO:
int graph_rect_is_cross(struct graph *cthis, struct point point)
{
    struct graph_rect *cthis_tf = (struct graph_rect *)cthis;
    return 0;
}

int graph_rect_move(struct graph *cthis,struct point base,struct point vector)
{
    struct graph_rect *cthis_tf = (struct graph_rect *)cthis;

    (cthis_tf->pstart).x += vector.x;
    (cthis_tf->pstart).y += vector.y;
    (cthis_tf->pend).x   += vector.x;
    (cthis_tf->pend).y   += vector.y;
    (cthis_tf->pa).x     += vector.x;
    (cthis_tf->pa).y     += vector.y;
    (cthis_tf->pb).x     += vector.x;
    (cthis_tf->pb).y     += vector.y;

    return 0;
}

//旋转（积点自定）
int graph_rect_rot(struct graph *cthis,struct point base,float angle)
{
    struct graph_rect *cthis_tf = (struct graph_rect *)cthis;

    //计算二维平面有基点旋转

    cthis_tf->pstart = calc_baserot(cthis_tf->pstart,base,angle);
    cthis_tf->pend   = calc_baserot(cthis_tf->pend,base,angle);
    cthis_tf->pa     = calc_baserot(cthis_tf->pa,base,angle);
    cthis_tf->pb     = calc_baserot(cthis_tf->pb,base,angle);

    return 0;
}

int graph_rect_resize(struct graph *cthis,struct point base,float rate)
{
    struct graph_rect *cthis_tf = (struct graph_rect *)cthis;

    cthis_tf->pstart = calc_scale(cthis_tf->pstart,base,rate);
    cthis_tf->pend   = calc_scale(cthis_tf->pend,base,rate);
    cthis_tf->pa     = calc_scale(cthis_tf->pa,base,rate);
    cthis_tf->pb     = calc_scale(cthis_tf->pb,base,rate);

    return 0;
}
struct point graph_rect_get_center(struct graph *cthis)
{
    struct graph_rect *cthis_tf = (struct graph_rect *)cthis;

    return POINT(((cthis_tf->pstart).x + (cthis_tf->pend).x) / 2,
                 ((cthis_tf->pend).x + (cthis_tf->pend).y) / 2);

}
struct graph *graph_rect_deepcopy(struct graph *cthis)
{
    struct graph_rect *cthis_tf = (struct graph_rect *)cthis;

    return (struct graph *)graph_rect_new(cthis_tf->pstart,cthis_tf->pend);
}

int graph_rect_draw_tag(struct graph *cthis,struct window * draw_object)
{
    struct graph_rect *cthis_tf = (struct graph_rect *)cthis;
    char tags[100];
    sprintf(tags,"R(%d)",cthis->object_id);

    //在图形重点画标记
    window_draw_tag(draw_object,calc_center(cthis_tf->pstart,cthis_tf->pend),tags);

    return 0;
}
int graph_rect_get_tag(struct graph *cthis, struct graph_description *out_info, char *out_info_str)
{
    return 0;
}