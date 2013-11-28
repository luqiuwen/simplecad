
//画布头文件


//简略类写法（无继承）

#pragma once

#include "graph.h"

struct drawarea {
    //画布文件
    struct window *window; //绘图对象

    struct graph **graph_objlist;  //绘图对象容器
    struct graph **graph_select;   //选中对象容器
    
    int graph_objlist_count;       //绘图对象容器中对象的数量
    int graph_objlist_max;         //绘图对象容器最大数量

    int graph_select_count;       //绘图选中对象容器中对象的数量
    int graph_select_max;         //绘图选中对象容器最大数量

    int is_showtag;                 //绘图时是否显示图像的TAG

    int height;
    int width;

    struct point statusbar;     //状态栏起始点位置
    char statusbar_info[__DRAWAREA_STATUSBAR_INFOTEXT_MAX__];
    int is_showbar;             //是否显示状态栏

};

struct drawarea * drawarea_new(struct window *window); //申请新的画布
int drawarea_delete(struct drawarea *cthis); //释放画布空间

int drawarea_insert_graph(struct drawarea *cthis,struct graph *gobj);  //添加图形
int drawarea_delete_graph(struct drawarea *cthis,struct graph *gobj);  //删除图形
int drawarea_draw(struct drawarea *cthis);          //绘制图形
int drawarea_showtags(struct drawarea *cthis);

int drawarea_clear(struct drawarea *cthis);     //清空画布
int drawarea_save_jpg(struct drawarea,char *picture_path);  //保存成位图

struct graph * drawarea_search(struct drawarea *cthis,int id);

int drawarea_select_insert(struct drawarea *cthis,struct graph *object);
int drawarea_select_empty(struct drawarea *cthis);
int drawarea_select_show_selected(struct drawarea *cthis);

int drawarea_statusbar_draw(struct drawarea *cthis);
int drawarea_statusbar_clear(struct drawarea *cthis);
int drawarea_statusbar_print(struct drawarea *cthis,char *msg);