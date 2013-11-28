
//绘图图形类

#pragma once



#include "common.h"
#include "point.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <pthread.h>

//接口
struct window {
    
    struct window_img *img;
    int height;
    int width;

    __FUNCTION__ int (*draw_line)(struct window * cthis,struct point start,struct point end);
    __FUNCTION__ int (*draw_circle)(struct window * cthis,struct point center,int radius);
    __FUNCTION__ int (*draw_point)(struct window * cthis,struct point point);
    __FUNCTION__ int (*draw_tag)(struct window *cthis,struct point point,char * msg);
    __FUNCTION__ int (*draw_eclipse)(struct window *cthis,
                        struct point center,int axes_x,int axes_y,float angle,float start_angle,float end_angle);
    __FUNCTION__ int (*zero)(struct window *cthis);

};

struct window * window_new(int height,int width);
void window_delete(struct window *cthis);
int window_draw_line(struct window * cthis,struct point start,struct point end);
int window_draw_circle(struct window *cthis, struct point center,int radius);
int window_draw_point(struct window *cthis, struct point point);
int window_draw_tag(struct window *cthis,struct point point ,char * msg);
int window_draw_eclipse(struct window *cthis,
                        struct point center,
                        int axes_x,int axes_y,
                        float angle,float start_angle,float end_angle);

int window_zero(struct window *cthis);


//---------------------------------------------------------------------------------

struct window_img {

    __DATA__ IplImage *img;

    __DATA__ int height;
    __DATA__ int width;
    __DATA__ int id;

    __DATA__ char title[120]; //窗口标题

    //线程

    pthread_t thread_id; //线程ID标志
    pthread_rwlock_t thread_lock; //锁（用于公共Img内存空间的访问）

    __FUNCTION__ int (*draw_line)(struct window_img *cthis,struct point start,struct point end);
    __FUNCTION__ int (*draw_circle)(struct window_img *cthis,struct point center,int radius);
    __FUNCTION__ int (*draw_point)(struct window_img *cthis,struct point point);
    __FUNCTION__ int (*draw_tag)(struct window_img *cthis,struct point point, char * msg);
    __FUNCTION__ int (*draw_eclipse)(struct window_img *cthis,
                        struct point center,int axes_x,int axes_y,float angle,float start_angle,float end_angle);

    __FUNCTION__ int (*zero)(struct window_img *cthis);

};

//成员函数
struct window_img * window_img_new(int size_x,int size_y);
int window_img_delete(struct window_img * cthis);
int window_img_init(struct window_img *cthis,int height,int width);
int window_img_exit(struct window_img *cthis);
int window_img_draw_point(struct window_img *cthis,struct point point);
int window_img_draw_line(struct window_img *cthis,struct point start,struct point end);
int window_img_draw_circle(struct window_img *cthis,struct point center,int radius);
int window_img_erase_point(struct window_img *cthis,struct point point);
int window_img_draw_tag(struct window_img *cthis,struct point point,char * msg);
int window_img_draw_eclipse(struct window_img *cthis,
                        struct point center,
                        int axes_x,int axes_y,
                        float angle,float start_angle,float end_angle);
int window_img_zero(struct window_img *cthis);

//线程管理

static void * window_img_thread(void *arg);