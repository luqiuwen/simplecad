
//基本图元库：直线

#include "common.h"
#include "point.h"
#include "graph.h"

struct graph_line {
//基本图元：直线
    struct graph graph; //继承自类Graph

    //直线起点
    __DATA__ struct point pstart;
    //直线终点
    __DATA__ struct point pend;

    //继承自graph的方法----------------------------------------------------------------------
    //添加子元：返回值：图元序号
    __FUNCTION__ int (*insert)(struct graph_line *cthis,struct graph_line *item);
    //删除子元
    __FUNCTION__ int (*del)(struct graph_line *cthis,int item_id);
    //返回子元序列
    __FUNCTION__ struct graph** (*get_child)(struct graph_line *cthis);
    //返回父元
    __FUNCTION__ struct graph* (*get_parent)(struct graph_line *cthis);
    //返回子代数量
    __FUNCTION__ int (*get_child_count)(struct graph_line *cthis);

    //覆写的函数------------------------------------------------------------------------------
    //构造函数
    __FUNCTION__ __OVERRIDE__ void (*init)(struct graph_line *cthis,struct point pstart,struct point pend);
    //析构函数
    __FUNCTION__ __OVERRIDE__ void (*exit)(struct graph_line *cthis);
    //绘制图形
    __FUNCTION__ __OVERRIDE__ int (*draw)(struct graph_line *cthis,struct window * draw_object);
    //判断是否相交
    __FUNCTION__ __OVERRIDE__ int (*is_cross)(struct graph_line *cthis,struct point point);
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

void graph_line_init(struct graph_line * cthis,struct point pstart,struct point pend);
void graph_line_exit(struct graph_line * cthis);
struct graph_line * graph_line_new(struct point pstart,struct point pend);
void graph_line_delete(struct graph_line * cthis);
int graph_line_insert(struct graph_line *cthis,struct graph_line *item);
int graph_line_del(struct graph_line *cthis,int item_id);
struct graph** graph_line_get_child(struct graph_line *cthis);
struct graph* graph_line_get_parent(struct graph_line *cthis);
int graph_line_get_child_count(struct graph_line *cthis);
int graph_line_draw(struct graph * cthis, struct window * draw_object);
int graph_line_is_cross(struct graph *cthis, struct point point);
int graph_line_move(struct graph *cthis,struct point base,struct point vector);
int graph_line_rot(struct graph *cthis,struct point base,float angle);
int graph_line_resize(struct graph *cthis,struct point base,float rate);
struct point graph_line_get_center(struct graph *cthis);
struct graph *graph_line_deepcopy(struct graph *cthis);
int graph_line_draw_tag(struct graph *cthis,struct window * draw_object);
int graph_line_get_tag(struct graph *cthis, struct graph_description *out_info, char *out_info_str);