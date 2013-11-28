
//Graph类的实现

#include <stdlib.h>
#include <assert.h>

#include "graph.h"
#include "common.h"

#define __GRAPH_DEFAULT_CHILD_MAX__ 10  //默认的子代对象缓冲区大小


//======================================================================================
//构造函数
void graph_init(struct graph *cthis)
{
    //注册函数
    cthis->init            = graph_init;
    cthis->exit            = graph_exit;
    cthis->insert          = graph_insert;
    cthis->del             = graph_del;
    cthis->get_child       = graph_get_child;
    cthis->get_parent      = graph_get_parent;
    cthis->get_child_count = graph_get_child_count;
    
    //虚函数
    cthis->draw            = NULL;
    cthis->is_cross        = NULL;
    cthis->move            = NULL;
    cthis->rot             = NULL;
    cthis->resize          = NULL;
    cthis->get_center      = NULL;
    cthis->deepcopy        = NULL;
    cthis->draw_tag        = NULL;
    cthis->get_tag         = NULL;
    
    //分配子对象缓冲区对象地址，初始化指针
    cthis->child_count     = 0;
    cthis->child_max       = __GRAPH_DEFAULT_CHILD_MAX__;
    cthis->parent          = NULL;
    cthis->child           = calloc(__GRAPH_DEFAULT_CHILD_MAX__,sizeof(struct graph *));

    //分配对象ID

    cthis->object_id   = (__static_graph_id)++;

    return;
}
//析构函数
void graph_exit(struct graph *cthis)
{
    free(cthis->child);
    free(cthis);

    return;
}

//======================================================================================

//添加图元
int graph_insert(struct graph *cthis,struct graph *item)
{
    assert(cthis->child_count <= cthis->child_max);  //断言：子代数量小于等于缓冲区大小
    
    if(cthis->child_count == cthis->child_max)   { //缓冲区满
        cthis->child_max += __GRAPH_DEFAULT_CHILD_MAX__;
        cthis->child   = realloc(cthis->child,cthis->child_max);  //扩大缓冲区
    }

    //修改子代对象的父亲指针
    item->parent = cthis;
    //添加图元到子代缓冲区
    cthis->child[cthis->child_count] = item;
    (cthis->child_count)++;

    return 0;
}

int graph_del(struct graph *cthis,int item_id)
{
    return 0;
}

//获得亲代图元对象指针
struct graph * graph_get_parent(struct graph *cthis)
{
    return cthis->parent;
}

//获得子代图元对象指针
struct graph ** graph_get_child(struct graph *cthis)
{
    return cthis->child;
}

//获得子代图元数量
int graph_get_child_count(struct graph *cthis)
{
    return cthis->child_count;
}

//虚函数动态绑定
int graph_draw(struct graph * cthis,struct window * draw_object)
{
    if(cthis->draw == NULL)
        return 0;
    cthis->draw(cthis,draw_object);
    return 0;
}
int graph_is_cross(struct graph *cthis,struct point point)
{
    if(cthis->is_cross == NULL)
        return 0;
    return cthis->is_cross(cthis,point);
}

int graph_move(struct graph *cthis,struct point base,struct point vector)
{
    if(cthis->move == NULL)
        return 0;
    return cthis->move(cthis,base,vector);
}
int graph_rot(struct graph *cthis,struct point base,float angle)
{
    if(cthis->rot == NULL)
        return 0;
    return cthis->rot(cthis,base,angle);
}
int graph_resize(struct graph *cthis,struct point base,float rate)
{
    if(cthis->resize == NULL)
        return 0;
    return cthis->resize(cthis,base,rate);
}

struct point graph_get_center(struct graph *cthis)
{
    if(cthis->get_center == NULL)
        return POINT(0,0);
    return cthis->get_center(cthis);
}

struct graph * graph_deepcopy(struct graph *cthis)
{
    if(cthis->deepcopy == NULL)
        return NULL;
    return cthis->deepcopy(cthis);
}
int graph_draw_tag(struct graph *cthis,struct window * draw_object)
{
    if(cthis->draw_tag == NULL)
        return 0;
    return cthis->draw_tag(cthis,draw_object);
}

int graph_get_tag(struct graph *cthis, struct graph_description *out_info, char *out_info_str)
{
    if(cthis->get_tag == NULL)
        return 0;
    return cthis->get_tag(cthis,out_info,out_info_str);
}

// struct graph * graph_new()
// {
//     //获取新的Graph对象
//     struct graph *self_new = (struct graph *)malloc(sizeof(graph));
//     graph_init(self_new);
//     return self_new;
// }

// int graph_delete(struct graph *cthis)
// {
//     //删除Graph对象
//     graph_exit(cthis);
//     free(cthis);
//     return 0;
// }
