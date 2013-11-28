
//基本图元库：圆

#include "common.h"
#include "point.h"
#include "graph.h"

struct graph_circle {
//基本图元：圆
    struct graph graph; //继承自类Graph

    //圆中点
    __DATA__ struct point center;
    //圆半径
    __DATA__ int radius;

    //继承自graph的方法----------------------------------------------------------------------
    //添加子元：返回值：图元序号
    __FUNCTION__ int (*insert)(struct graph_circle *cthis,struct graph_circle *item);
    //删除子元
    __FUNCTION__ int (*del)(struct graph_circle *cthis,int item_id);
    //返回子元序列
    __FUNCTION__ struct graph** (*get_child)(struct graph_circle *cthis);
    //返回父元
    __FUNCTION__ struct graph* (*get_parent)(struct graph_circle *cthis);
    //返回子代数量
    __FUNCTION__ int (*get_child_count)(struct graph_circle *cthis);

    //覆写的函数------------------------------------------------------------------------------
    //构造函数
    __FUNCTION__ __OVERRIDE__ void (*init)(struct graph_circle *cthis,struct point center,int radius);
    //析构函数
    __FUNCTION__ __OVERRIDE__ void (*exit)(struct graph_circle *cthis);
    //绘制图形
    __FUNCTION__ __OVERRIDE__ int (*draw)(struct graph_circle *cthis,struct window * draw_object);
    //判断是否相交
    __FUNCTION__ __OVERRIDE__ int (*is_cross)(struct graph_circle *cthis,struct point point);
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

void graph_circle_init(struct graph_circle * cthis,struct point center,int radius);
void graph_circle_exit(struct graph_circle * cthis);
struct graph_circle * graph_circle_new(struct point center,int radius);
void graph_circle_delete(struct graph_circle * cthis);
int graph_circle_insert(struct graph_circle *cthis,struct graph_circle *item);
int graph_circle_del(struct graph_circle *cthis,int item_id);
struct graph** graph_circle_get_child(struct graph_circle *cthis);
struct graph* graph_circle_get_parent(struct graph_circle *cthis);
int graph_circle_get_child_count(struct graph_circle *cthis);
int graph_circle_draw(struct graph * cthis, struct window * draw_object);
int graph_circle_is_cross(struct graph *cthis, struct point point);
int graph_circle_move(struct graph *cthis,struct point base,struct point vector);
int graph_circle_rot(struct graph *cthis,struct point base,float angle);
int graph_circle_resize(struct graph *cthis,struct point base,float rate);
struct point graph_circle_get_center(struct graph *cthis);
struct graph * graph_circle_deepcopy(struct graph *cthis);
int graph_circle_draw_tag(struct graph *cthis,struct window * draw_object);
int graph_circle_get_tag(struct graph *cthis, struct graph_description *out_info, char *out_info_str);