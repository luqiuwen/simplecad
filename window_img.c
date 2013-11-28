#include <stdio.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <pthread.h>
#include <unistd.h>

#include "window.h"
#include "point.h"
#include "config.h"

#define __CV_WINDOW_NAME__ "CAD%d"

static int window_img_id = 0;
static pthread_mutex_t __window_img_thread_lock = PTHREAD_MUTEX_INITIALIZER;

//类：window_img的实现代码 =========================================================

//###############################################################################

struct window_img * window_img_new(int height,int width)
{
    
    struct window_img * cthis = (struct window_img *)malloc(sizeof(struct window_img));

    if(!cthis)
        return NULL;

    window_img_init(cthis,height,width);

    return cthis;
}

int window_img_delete(struct window_img * cthis)
{
    window_img_exit(cthis);
    free(cthis);
    return 0;
}

//###############################################################################

//构造函数
int window_img_init(struct window_img *cthis,int height,int width)
{
    cthis->img    = cvCreateImage(cvSize(height,width),IPL_DEPTH_8U,1);
    cthis->height = height;
    cthis->width  = width;

    cthis->draw_line    = window_img_draw_line;
    cthis->draw_point   = window_img_draw_point;
    cthis->draw_circle  = window_img_draw_circle;
    cthis->draw_tag     = window_img_draw_tag;
    cthis->draw_eclipse = window_img_draw_eclipse;
    cthis->zero         = window_img_zero;
    
    cthis->id           = window_img_id++;

    //初始化线程、锁

    //?? 此处有问题

    
    sprintf(cthis->title,__CV_WINDOW_NAME__,cthis->id);
    cvNamedWindow(cthis->title,CV_WINDOW_AUTOSIZE);

    pthread_rwlock_init(&(cthis->thread_lock),NULL);

    int error;
    error = pthread_create(&(cthis->thread_id),NULL,window_img_thread,(void *)cthis);

    if(error)
        return -1;



    // printf("%s",cthis->title);

    //TODO:Fix Bug.


    return 0;
}

int window_img_exit(struct window_img *cthis)
{
    
    cvReleaseImage(&(cthis->img));
    //销毁锁
    pthread_rwlock_destroy(&(cthis->thread_lock));


    cthis->height = 0;
    cthis->width  = 0;
    return 0;
}

//绘制点
int window_img_draw_point(struct window_img *cthis,struct point point)
{
    //判断点是否在绘图空间中

    int height = cthis->height;
    int width  = cthis->width;

    int is_range_x = (point.x <= width) && (point.x >= 0);
    int is_range_y = (point.y <= height) && (point.y >= 0);

    //点不在绘图空间中
    if(!(is_range_x && is_range_y))
        return -1;

    pthread_rwlock_wrlock(&(cthis->thread_lock));
    cvSet2D(cthis->img,point.y,point.x,CV_RGB(255,255,255)); //设置为白色
    pthread_rwlock_unlock(&(cthis->thread_lock));

    return 0;
}
//擦除点
int window_img_erase_point(struct window_img *cthis,struct point point)
{
    //判断点是否在绘图空间中

    int height = cthis->height;
    int width  = cthis->width;

    int is_range_x = (point.x <= height) && (point.x >= 0);
    int is_range_y = (point.y <= width) && (point.y >= 0);

    //点不在绘图空间中
    if(!(is_range_x && is_range_y))
        return -1;

    pthread_rwlock_wrlock(&(cthis->thread_lock));
    cvSet2D(cthis->img,point.y,point.x,CV_RGB(0,0,0)); //设置为黑色
    pthread_rwlock_unlock(&(cthis->thread_lock));

    return 0;
}

//画直线
int window_img_draw_line(struct window_img *cthis,struct point start,struct point end)
{
    //画线
    pthread_rwlock_wrlock(&(cthis->thread_lock));
    cvLine(cthis->img,
           cvPoint(start.x,start.y),
           cvPoint(end.x,end.y),
           CV_RGB(255,255,255),1,8,0);
    pthread_rwlock_unlock(&(cthis->thread_lock));

    return 0;
}

int window_img_draw_circle(struct window_img *cthis,struct point center,int radius)
{
    pthread_rwlock_wrlock(&(cthis->thread_lock));
    cvCircle(cthis->img,cvPoint(center.x,center.y),radius,CV_RGB(255,255,255),1,8,0);
    pthread_rwlock_unlock(&(cthis->thread_lock));

    return 0;
}

int window_img_draw_tag(struct window_img *cthis,struct point point ,char * msg)
{
    //画图元标志
    struct CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX_SMALL,__TAG_FONT_SCALE__,__TAG_FONT_SCALE__,0,1,8);
    pthread_rwlock_wrlock(&(cthis->thread_lock));
        cvPutText(cthis->img,
                  msg,
                  cvPoint(point.x,point.y),
                  &font,
                  __TAG_FONT_COLOR__);
    pthread_rwlock_unlock(&(cthis->thread_lock));
    return 0;
}

int window_img_draw_eclipse(struct window_img *cthis,struct point center,
                            int axes_x,int axes_y,float angle,float start_angle,float end_angle)
{
    //画椭圆
    pthread_rwlock_wrlock(&(cthis->thread_lock));
    cvEllipse(cthis->img,cvPoint(center.x,center.y),
                         cvSize(axes_x,axes_y),
                         angle,
                         start_angle,end_angle,
                         CV_RGB(255,255,255),1,8,0);
    
    pthread_rwlock_unlock(&(cthis->thread_lock));

    return 0;
}


int window_img_zero(struct window_img *cthis)
{
    cvZero(cthis->img);
    return 0;
}

//线程函数：动态刷新GUI窗口

static void * window_img_thread(void *arg)
{
    struct window_img *cthis = (struct window_img *)arg;

    IplImage *bufimg = cvCreateImage(cvSize(cthis->height,cthis->width),IPL_DEPTH_8U,1);

    while(1)  {
        //动态刷新GUI
        //复制图像缓冲区,请求读锁

         //线程安全锁
        // while(pthread_mutex_trylock(&__window_img_thread_lock) != 0)
            // ;

        //TODO:线程安全问题：OpenCV Highui 库是否是线程安全的？

        pthread_rwlock_rdlock(&(cthis->thread_lock));
        cvCopyImage(cthis->img,bufimg);
        pthread_rwlock_unlock(&(cthis->thread_lock));
        cvShowImage(cthis->title,bufimg);
        cvWaitKey(15);
        //释放读锁
        // pthread_mutex_unlock(&__window_img_thread_lock);

        // printf("%s\n",cthis->title);
    }

    cvReleaseImage(&bufimg);
    cvDestroyWindow(cthis->title);

    return ((void *)0);

}


//----------------------------------------------------------------------------------