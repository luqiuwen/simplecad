// Graph接口

#pragma once

#include "config.h"
#include "window.h"
#include "point.h"
#include "common.h"

static int __static_graph_id = 0; //全局变量：待分配的Graph ID序号


struct graph_description {
    //图形通用描述信息
    int id; // 图元ID序号
    char name[__GRAPH_DESCRIPTION_NAME_MAX__];  //图元名称
    struct point center;                        //图元中心

};

//图形图元接口(抽象类)
struct graph {

    __DATA__    int object_id;  //图元代号

    __DATA__ struct graph *    parent;
    __DATA__ struct graph **   child;
    __DATA__    int child_count;
    __DATA__    int child_max;
    __DATA__ struct graph_description info;

    //构造函数
    __FUNCTION__ void (*init)(struct graph *cthis);
    //析构函数
    __FUNCTION__ void (*exit)(struct graph *cthis);
    //添加子元：返回值：图元序号
    __FUNCTION__ int (*insert)(struct graph *cthis,struct graph *item);
    //删除子元
    __FUNCTION__ int (*del)(struct graph *cthis,int item_id);
    //返回子元序列
    __FUNCTION__ struct graph** (*get_child)(struct graph *cthis);
    //返回父元
    __FUNCTION__ struct graph* (*get_parent)(struct graph *cthis);
    //返回子代数量
    __FUNCTION__ int (*get_child_count)(struct graph *cthis);

    //虚函数:
    //绘制图形
    __FUNCTION__ __VIRTUAL__ int (*draw)(struct graph *cthis,struct window * draw_object);
    //判断是否相交
    __FUNCTION__ __VIRTUAL__ int (*is_cross)(struct graph *cthis,struct point point);
    //平移
    __FUNCTION__ __VIRTUAL__ int (*move)(struct graph *cthis,struct point base,struct point vector);
    //旋转
    __FUNCTION__ __VIRTUAL__ int (*rot)(struct graph *cthis,struct point base,float angle);
    //缩放
    __FUNCTION__ __VIRTUAL__ int (*resize)(struct graph *cthis,struct point base,float rate);
    //获得图形中心
    __FUNCTION__ __VIRTUAL__ struct point (*get_center)(struct graph *cthis);
    //复制自身
    __FUNCTION__ __VIRTUAL__ struct graph *(*deepcopy)(struct graph *cthis);
    //在画板上画出图元标志
    __FUNCTION__ __VIRTUAL__ int (*draw_tag)(struct graph *cthis,struct window * draw_object);
    //返回图元信息
    __FUNCTION__ __VIRTUAL__ int (*get_tag)(struct graph *cthis,
                                            struct graph_description *out_info,
                                            char *out_info_str);
};

//----------------------------------------------------------------------------------------

void graph_init(struct graph *cthis);
void graph_exit(struct graph *cthis);
int graph_insert(struct graph *cthis,struct graph *item);
int graph_del(struct graph *cthis,int item_id);
struct graph * graph_get_parent(struct graph *cthis);
struct graph ** graph_get_child(struct graph *cthis);
int graph_get_child_count(struct graph *cthis);


//----------------------------------------------------------------------------------------
//虚函数
int graph_draw(struct graph * cthis,struct window * draw_object);
int graph_is_cross(struct graph *cthis,struct point point);
int graph_move(struct graph *cthis,struct point base,struct point vector);
int graph_rot(struct graph *cthis,struct point base,float angle);
int graph_resize(struct graph *cthis,struct point base,float rate);
struct point graph_get_center(struct graph *cthis);
struct graph *graph_deepcopy(struct graph *cthis);
int graph_draw_tag(struct graph *cthis,struct window * draw_object);
int graph_get_tag(struct graph *cthis, struct graph_description *out_info, char *out_info_str);