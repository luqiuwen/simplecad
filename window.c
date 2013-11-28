
#include "window.h"
#include "assert.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <pthread.h>

//Window的构造函数
struct window * window_new(int height,int width)
{
    struct window *cthis = malloc(sizeof(struct window));
    if(!cthis)
        return NULL;
    //注册函数
    cthis->draw_line    = window_draw_line;
    cthis->draw_circle  = window_draw_circle;
    cthis->draw_point   = window_draw_point;
    cthis->draw_tag     = window_draw_tag;
    cthis->zero         = window_zero;
    cthis->draw_eclipse = window_draw_eclipse;

    cthis->height      = height;
    cthis->width       = width;
    
    //注册底层对象

    cthis->img = window_img_new(height,width);
    assert(cthis->img != NULL);

    return cthis;
}

void window_delete(struct window *cthis)
{
    window_img_delete(cthis->img);
    free(cthis);

    return;
}

//画线
int window_draw_line(struct window * cthis,struct point start,struct point end)
{
    return (cthis->img)->draw_line(cthis->img,start,end);
}
//画园
int window_draw_circle(struct window *cthis, struct point center,int radius)
{
    return (cthis->img)->draw_circle(cthis->img,center,radius);
}
//画点
int window_draw_point(struct window *cthis, struct point point)
{
    return (cthis->img)->draw_point(cthis->img,point);
}

int window_draw_tag(struct window *cthis,struct point point ,char * msg)
{
    return (cthis->img)->draw_tag(cthis->img,point,msg);
}

int window_zero(struct window * cthis)
{
    return (cthis->img)->zero(cthis->img);
}

int window_draw_eclipse(struct window *cthis,struct point center,
                        int axes_x,int axes_y,float angle,float start_angle,float end_angle)
{
    return (cthis->img)->draw_eclipse(cthis->img,center,axes_x,axes_y,angle,start_angle,end_angle);
}