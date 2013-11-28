
//基本图元库：椭圆 定义

#include "common.h"
#include "point.h"
#include "graph.h"

struct graph_eclipse {
//基本图元：圆
    struct graph graph; //继承自类Graph

    //圆中点
    __DATA__ struct point center;

    //椭圆半径
    __DATA__ int axes_x;
    __DATA__ int axes_y;
    __DATA__ float angle; //倾角

    //继承自graph的方法----------------------------------------------------------------------
    //添加子元：返回值：图元序号
    __FUNCTION__ int (*insert)(struct graph_eclipse *cthis,struct graph_eclipse *item);
    //删除子元
    __FUNCTION__ int (*del)(struct graph_eclipse *cthis,int item_id);
    //返回子元序列
    __FUNCTION__ struct graph** (*get_child)(struct graph_eclipse *cthis);
    //返回父元
    __FUNCTION__ struct graph* (*get_parent)(struct graph_eclipse *cthis);
    //返回子代数量
    __FUNCTION__ int (*get_child_count)(struct graph_eclipse *cthis);

    //覆写的函数------------------------------------------------------------------------------
    //构造函数
    __FUNCTION__ __OVERRIDE__ void (*init)(struct graph_eclipse * cthis,struct point center,int axes_x,int axes_y,float angle);
    //析构函数
    __FUNCTION__ __OVERRIDE__ void (*exit)(struct graph_eclipse *cthis);
    //绘制图形
    __FUNCTION__ __OVERRIDE__ int (*draw)(struct graph_eclipse *cthis,struct window * draw_object);
    //判断是否相交
    __FUNCTION__ __OVERRIDE__ int (*is_cross)(struct graph_eclipse *cthis,struct point point);
    //平移
    __FUNCTION__ __OVERRIDE__ int (*move)(struct graph *cthis,struct point base,struct point vector);
    //旋转
    __FUNCTION__ __OVERRIDE__ int (*rot)(struct graph *cthis,struct point base,float angle);
    //缩放
    __FUNCTION__ __OVERRIDE__ int (*resize)(struct graph *cthis,struct point base,float rate);
    //获取点
    __FUNCTION__ __OVERRIDE__ struct point (*get_center)(struct graph *cthis);
    //复制自身
    __FUNCTION__ __OVERRIDE__ struct graph *(*deepcopy)(struct graph *cthis);
    //在画板上画出图元标志
    __FUNCTION__ __OVERRIDE__ int (*draw_tag)(struct graph *cthis,struct window * draw_object);
    //返回图元信息
    __FUNCTION__ __OVERRIDE__ int (*get_tag)(struct graph *cthis,
                                            struct graph_description *out_info,
                                            char *out_info_str);

};

void graph_eclipse_init(struct graph_eclipse * cthis,struct point center,int axes_x,int axes_y,float angle);
void graph_eclipse_exit(struct graph_eclipse * cthis);
struct graph_eclipse * graph_eclipse_new(struct point center,int axes_x,int axes_y,float angle);
void graph_eclipse_delete(struct graph_eclipse * cthis);
int graph_eclipse_insert(struct graph_eclipse *cthis,struct graph_eclipse *item);
int graph_eclipse_del(struct graph_eclipse *cthis,int item_id);
struct graph** graph_eclipse_get_child(struct graph_eclipse *cthis);
struct graph* graph_eclipse_get_parent(struct graph_eclipse *cthis);
int graph_eclipse_get_child_count(struct graph_eclipse *cthis);
int graph_eclipse_draw(struct graph * cthis, struct window * draw_object);
int graph_eclipse_is_cross(struct graph *cthis, struct point point);
int graph_eclipse_move(struct graph *cthis,struct point base,struct point vector);
int graph_eclipse_rot(struct graph *cthis,struct point base,float angle);
int graph_eclipse_resize(struct graph *cthis,struct point base,float rate);
struct point graph_eclipse_get_center(struct graph *cthis);
struct graph * graph_eclipse_deepcopy(struct graph *cthis);
int graph_eclipse_draw_tag(struct graph *cthis,struct window * draw_object);
int graph_eclipse_get_tag(struct graph *cthis, struct graph_description *out_info, char *out_info_str);