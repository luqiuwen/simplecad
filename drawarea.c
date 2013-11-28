
#include "drawarea.h"
#include "config.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>



//获得新的画布对象
struct drawarea * drawarea_new(struct window *window)
{
    struct drawarea * cthis = (struct drawarea *)malloc(sizeof(struct drawarea));
    if(!cthis)
        return NULL;
    //初始化内部成员
    cthis->window              = window;
    cthis->graph_objlist_count = 0;
    cthis->graph_objlist_max   = __DRAWAREA_GRAPH_OBJ_DEFAULT_MAX__;
    cthis->graph_objlist       = (struct graph **)calloc(__DRAWAREA_GRAPH_OBJ_DEFAULT_MAX__,sizeof(struct graph *));
    
    cthis->graph_select        = (struct graph **)calloc(__DRAWAREA_GRAPH_OBJ_DEFAULT_MAX__,sizeof(struct graph *));
    cthis->graph_select_count  = 0;
    cthis->graph_select_max    = __DRAWAREA_GRAPH_OBJ_DEFAULT_MAX__;
    
    cthis->is_showtag          = 0;
    
    cthis->height              = (cthis->window)->height;
    cthis->width               = (cthis->window)->width;
    
    cthis->statusbar           = POINT(30,50);
    cthis->statusbar_info[0]   = 0;
    cthis->is_showbar          = 1;

    return cthis;
}

//状态栏提示：
int drawarea_statusbar_draw(struct drawarea *cthis)
{
    window_draw_tag(cthis->window,cthis->statusbar,cthis->statusbar_info);
    return 0;
}

int drawarea_statusbar_clear(struct drawarea *cthis)
{
    cthis->statusbar_info[0] = 0;
    return 0;
}

int drawarea_statusbar_print(struct drawarea *cthis,char *msg)
{
    strncpy(cthis->statusbar_info,msg,__DRAWAREA_STATUSBAR_INFOTEXT_MAX__);
    return 0;
}

//添加选中的对象
int drawarea_select_insert(struct drawarea *cthis,struct graph *object)
{
    assert(cthis->graph_select_count <= cthis->graph_select_max);
    if(cthis->graph_select_count == cthis->graph_select_max) 
    {
        //空间不够
        cthis->graph_select_max += __DRAWAREA_GRAPH_OBJ_DEFAULT_MAX__;
        cthis->graph_select = (struct graph **)realloc(cthis->graph_select,
                                                        cthis->graph_select_max 
                                                        * sizeof(struct graph *));
    }

    //查找对象在队列中是否已经存在

    for(int i = 0; i < cthis->graph_select_count ; i++)
        if(((cthis->graph_select)[i])->object_id == object->object_id)  //对象已经存在
            return 0;

    cthis->graph_select[cthis->graph_select_count] = object;
    (cthis->graph_select_count)++;

    return 0;
}

int drawarea_delete_graph(struct drawarea *cthis,struct graph *gobj)
{
    //删除指定的对象

    int delete_id = -1;

    for(int i = 0; i < cthis->graph_objlist_count ; i++)
        if(((cthis->graph_objlist)[i]) == gobj)
        {
            delete_id = i;
            break;
        }

    if(delete_id == -1 )
        //未找到删除的对象：指针错误
        return -1;

    graph_exit((cthis->graph_objlist)[delete_id]);

    //从队列中删除对象指针
    for(int i = delete_id; i < cthis->graph_objlist_count - 1; i++)
        (cthis->graph_objlist)[i] = (cthis->graph_objlist)[i+1];
    
    (cthis->graph_objlist_count)--; // 减少计数

    return 0;
}
//清空选中队列
int drawarea_select_empty(struct drawarea *cthis)
{
    memset(cthis->graph_select,0,sizeof(struct graph *)*(cthis->graph_select_count));
    cthis->graph_select_count = 0;

    drawarea_statusbar_clear(cthis);

    return 0;
}

int drawarea_select_show_selected(struct drawarea *cthis)
{
    //显示当前选中的图形对象ID

    char status_tmp[__DRAWAREA_STATUSBAR_INFOTEXT_MAX__];

    strcpy(status_tmp,"SELECTED:");

    for(int i = 0; i < cthis->graph_select_count ; i++)
    {
        char tmpchar[50];
        sprintf(tmpchar,"%d  ",((cthis->graph_select)[i])->object_id);
        strcat(status_tmp,tmpchar);
    }

    drawarea_statusbar_print(cthis,status_tmp);

    return 0;
}

struct graph * drawarea_search(struct drawarea *cthis,int id)
{
    for(int i = 0; i < cthis->graph_objlist_count ; i++)
        if((((cthis->graph_objlist)[i])->object_id) == id)
            return ((cthis->graph_objlist)[i]);
    return NULL;
}

int drawarea_delete(struct drawarea *cthis)
{
    //TODO:存在内存泄露的可能
    //TODO:释放存在队列中的对象本身的内存
    free(cthis->graph_objlist);
    free(cthis);

    return 0;
}

int drawarea_insert_graph(struct drawarea *cthis,struct graph *gobj)
{
    //断言：队列中对象的数量应当小于等于队列容量
    assert(cthis->graph_objlist_count <= cthis->graph_objlist_max);
    if(cthis->graph_objlist_count == cthis->graph_objlist_max) 
    {
        //空间不够
        cthis->graph_objlist_max += __DRAWAREA_GRAPH_OBJ_DEFAULT_MAX__;
        cthis->graph_objlist = (struct graph **)realloc(cthis->graph_objlist,
                                                        cthis->graph_objlist_max 
                                                        * sizeof(struct graph *));
    }
    //添加对象
    cthis->graph_objlist[cthis->graph_objlist_count] = gobj;
    (cthis->graph_objlist_count)++;
    return 0;        
}
//画标记
int drawarea_showtags(struct drawarea *cthis)
{
    for(int i = 0; i < cthis->graph_objlist_count ; i++)
        ((cthis->graph_objlist)[i])->draw_tag(cthis->graph_objlist[i],cthis->window);

    return 0;
}


int drawarea_draw(struct drawarea *cthis)
{
    //清空缓冲区
    window_zero(cthis->window);

    //重画
    for(int i = 0; i < cthis->graph_objlist_count ; i++)
        ((cthis->graph_objlist)[i])->draw(cthis->graph_objlist[i],cthis->window);

    if(cthis->is_showtag)
        drawarea_showtags(cthis);
    if(cthis->is_showbar)
        drawarea_statusbar_draw(cthis);

    return 0;
}