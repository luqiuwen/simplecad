
//CMD命令解释器执行函数（钩子函数）

#include <stdlib.h>
#include <stdio.h>

#include "cmdcontrol.h"

#include "config.h"
#include "common.h"
#include "command.h"
#include "window.h"
#include "drawarea.h"

#include "circle.h"
#include "line.h"
#include "rect.h"
#include "math.h"
#include "eclipse.h"
#include "arc.h"

//钩子函数数组
int (*cmd_shell_handle_func[__CMD_SHELL_MAX_HANDLE_COUNT__])(struct command *cthis,int argc,char *argv[]);
//钩子函数数量
int cmd_shell_handle_func_count;

int cmd_shell_handle_new(struct command *cthis,int argc,char *argv[])
{
    HANDLE_RESPONSE("new");

    cthis->window = window_new(atoi(argv[1]),atoi(argv[2]));
    cthis->drawarea = drawarea_new(cthis->window);

    return 0;
}

int cmd_shell_handle_draw_line(struct command *cthis,int argc,char *argv[])
{
    HANDLE_RESPONSE("l");
    CHECK_PNUM(4);

    int p1,p2,p3,p4;

    GET_INPUT(argv[1],"%d",p1);
    GET_INPUT(argv[2],"%d",p2);
    GET_INPUT(argv[3],"%d",p3);
    GET_INPUT(argv[4],"%d",p4);

    struct graph *line = (struct graph *)graph_line_new(POINT(p1,p2),
                                                        POINT(p3,p4));

    drawarea_insert_graph(cthis->drawarea,line);
    drawarea_draw(cthis->drawarea);

    return 0;
}

int cmd_shell_handle_draw_rect(struct command *cthis,int argc,char *argv[])
{
    HANDLE_RESPONSE("r");
    CHECK_PNUM(4);

    int p1,p2,p3,p4;

    GET_INPUT(argv[1],"%d",p1);
    GET_INPUT(argv[2],"%d",p2);
    GET_INPUT(argv[3],"%d",p3);
    GET_INPUT(argv[4],"%d",p4);

    struct graph *rect = (struct graph *)graph_rect_new(POINT(p1,p2),
                                                        POINT(p3,p4));

    drawarea_insert_graph(cthis->drawarea,rect);
    drawarea_draw(cthis->drawarea);

    return 0;
}

//画圆
int cmd_shell_handle_draw_circle(struct command *cthis,int argc,char *argv[])
{
    HANDLE_RESPONSE("c");
    CHECK_PNUM(3);

    int r,p1,p2;
    GET_INPUT(argv[1],"%d",p1);
    GET_INPUT(argv[2],"%d",p2);
    GET_INPUT(argv[3],"%d",r);

    struct graph *circle = (struct graph *)graph_circle_new(POINT(p1,p2),r);
    drawarea_insert_graph(cthis->drawarea,circle);
    drawarea_draw(cthis->drawarea);

    return 0;
}
//画椭圆
int cmd_shell_handle_draw_eclipse(struct command *cthis,int argc,char *argv[])
{
    HANDLE_RESPONSE("e");
    CHECK_PNUM(5);
    int ax,ay,cx,cy;
    float angle;
    GET_INPUT(argv[1],"%d",ax);
    GET_INPUT(argv[2],"%d",ay);
    GET_INPUT(argv[3],"%d",cx);
    GET_INPUT(argv[4],"%d",cy);
    GET_INPUT(argv[5],"%f",angle);

    struct graph *eclipse = (struct graph *)graph_eclipse_new(POINT(cx,cy),ax,ay,angle);
    drawarea_insert_graph(cthis->drawarea,eclipse);
    drawarea_draw(cthis->drawarea);

    return 0;
}

//画弧线
int cmd_shell_handle_draw_arc(struct command *cthis,int argc,char *argv[])
{
    HANDLE_RESPONSE("a");
    CHECK_PNUM(6);

    int center_x,center_y,radius;
    float start_angle,end_angle,angle;

    GET_INPUT(argv[1],"%d",center_x);   
    GET_INPUT(argv[2],"%d",center_y);   
    GET_INPUT(argv[3],"%d",radius);   
    GET_INPUT(argv[4],"%f",start_angle);   
    GET_INPUT(argv[5],"%f",end_angle);   
    GET_INPUT(argv[6],"%f",angle);

    struct graph *arc = (struct graph *)graph_arc_new(POINT(center_x,center_y),
                                                      radius,
                                                      start_angle,end_angle,angle);
    drawarea_insert_graph(cthis->drawarea,arc);
    drawarea_draw(cthis->drawarea);

    return 0;
}

int cmd_shell_handle_tag(struct command *cthis,int argc,char *argv[])
{
    HANDLE_RESPONSE("tag");
    CHECK_PNUM(1);

    if(argv[1][0] == '0')
        (cthis->drawarea)->is_showtag = 0;
    else
        (cthis->drawarea)->is_showtag = 1;

    drawarea_draw(cthis->drawarea);

    return 0;
}

int cmd_shell_handle_move(struct command *cthis,int argc,char *argv[])
{
    HANDLE_RESPONSE("move");
    CHECK_PNUM(2);

    int vec_x,vec_y;

    GET_INPUT(argv[1],"%d",vec_x);
    GET_INPUT(argv[2],"%d",vec_y);

    for(int i = 0; i < (cthis->drawarea)->graph_select_count ; i++)
        (((cthis->drawarea)->graph_select)[i])->move(((cthis->drawarea)->graph_select)[i],POINT(0,0),POINT(vec_x,vec_y));

    drawarea_draw(cthis->drawarea); //重画
    return 0;
}

int cmd_shell_handle_rot(struct command *cthis,int argc,char *argv[])
{
    HANDLE_RESPONSE("rot");
    CHECK_PNUM(3);

    int base_x,base_y;
    float angle;

    GET_INPUT(argv[1],"%d",base_x);
    GET_INPUT(argv[2],"%d",base_y);
    GET_INPUT(argv[3],"%f",angle);

    for(int i = 0; i < (cthis->drawarea)->graph_select_count ; i++)
        (((cthis->drawarea)->graph_select)[i])->rot(((cthis->drawarea)->graph_select)[i],POINT(base_x,base_y),angle);

    drawarea_draw(cthis->drawarea);
    return 0;
}


int cmd_shell_handle_resize(struct command *cthis,int argc,char *argv[])
{
    HANDLE_RESPONSE("resize");

    CHECK_PNUM(3);

    int base_x,base_y;
    float rate;

    GET_INPUT(argv[1],"%d",base_x);
    GET_INPUT(argv[2],"%d",base_y);
    GET_INPUT(argv[3],"%f",rate);

    for(int i = 0; i < (cthis->drawarea)->graph_select_count ; i++)
        (((cthis->drawarea)->graph_select)[i])->resize(((cthis->drawarea)->graph_select)[i],POINT(base_x,base_y),fabs(rate));

    drawarea_draw(cthis->drawarea);

    return 0;
}

//删除图形
int cmd_shell_handle_del(struct command *cthis,int argc,char *argv[])
{
    HANDLE_RESPONSE("del");

    CHECK_PNUM(0);

    for(int i = 0; i < (cthis->drawarea)->graph_select_count ; i++)
        drawarea_delete_graph(cthis->drawarea,((cthis->drawarea)->graph_select)[i]);

    drawarea_select_empty(cthis->drawarea);

    drawarea_draw(cthis->drawarea);

    return 0;
}

int cmd_shell_handle_copy(struct command *cthis,int argc,char *argv[])
{
    HANDLE_RESPONSE("copy");
    CHECK_PNUM(0);

    for(int i = 0; i < (cthis->drawarea)->graph_select_count ; i++)
    {
        struct graph * tmp = (((cthis->drawarea)->graph_select)[i])->deepcopy(((cthis->drawarea)->graph_select)[i]);
        drawarea_insert_graph(cthis->drawarea,tmp);
    }

    drawarea_draw(cthis->drawarea);

    return 0;
}

int cmd_shell_handle_select(struct command *cthis,int argc,char *argv[])
{
    HANDLE_RESPONSE("select");

    if(argv[1][0] == '-')
    {
        drawarea_select_empty(cthis->drawarea); //清空选中区
        drawarea_statusbar_clear(cthis->drawarea); //清空状态栏
        drawarea_draw(cthis->drawarea);

        return 0;
    }

    int select_obj_count = argc - 1; //选择对象的数量
    for(int i = 0; i < select_obj_count ; i++)
    {
        //查找对象

        int id;
        GET_INPUT(argv[i+1],"%d",id);

        struct graph * selected = drawarea_search(cthis->drawarea,id);
        if(selected == NULL)
        {
            //未找到
            cmd_shell_error(cthis,3,"Cannot find selected graph object.");
            return 0;
        }

        drawarea_select_insert(cthis->drawarea,selected);
    }

    //在状态栏上显示选中的对象
    drawarea_select_show_selected(cthis->drawarea);
    drawarea_draw(cthis->drawarea);

    return 0;
}

int cmd_shell_handle_statusc(struct command *cthis,int argc,char *argv[])
{
    HANDLE_RESPONSE("#status");

    drawarea_statusbar_print(cthis->drawarea,argv[1]);
    drawarea_draw(cthis->drawarea);

    return 0;
}

//以下是关于命令解释器的操作函数

int cmd_shell_handle_create(struct command *cthis,int argc,char *argv[])
{
    //新建工作区

    HANDLE_RESPONSE("@create");
    CHECK_PNUM(0);

    struct command *obj = command_new(stdin,stdout,NULL);
    command_object_insert(obj);
    command_use(obj->id);

    return 0;
}

int cmd_shell_handle_use(struct command *cthis,int argc,char *argv[])
{
    //切换工作区
    HANDLE_RESPONSE("@use");
    CHECK_PNUM(1);

    int use_id;
    GET_INPUT(argv[1],"%d",use_id);

    if(command_use(use_id))
        cmd_shell_error(cthis,8,"Cannot use the workarea.");

    return 0;
}
int cmd_shell_handle_rfile(struct command *cthis,int argc,char *argv[])
{
    //从文件读指令集并执行
    HANDLE_RESPONSE("@load");
    CHECK_PNUM(1);

    //添加命令控制器
    struct command * obj = factory_command_file(argv[1]);

    if(!obj) {
        cmd_shell_error(cthis,7,"Cannot open operation file.");
        return 0;
    }

    command_object_insert(obj);
    command_use(obj->id);

    return 0;
}

int cmd_shell_handle_record(struct command *cthis,int argc,char *argv[])
{
    HANDLE_RESPONSE("@rdstart");
    CHECK_PNUM(1);

    FILE *fp_aux = fopen(argv[1],"w");
    if(!fp_aux)  {
        cmd_shell_error(cthis,8,"Cannot save record file");
        return 0;
    }

    cthis->stream_aux = fp_aux;

    return 0;
}

int cmd_shell_handle_recordstop(struct command *cthis,int argc,char *argv[])
{
    HANDLE_RESPONSE("@rdstop");
    CHECK_PNUM(0);

    if(!cthis->stream_aux)
        return 0;

    fclose(cthis->stream_aux);
    (cthis->stream_aux) = NULL;

    return 0;
}

int cmd_shell_handle_exit(struct command *cthis,int argc,char *argv[])
{
    HANDLE_RESPONSE("exit");
    CHECK_PNUM(0);

    exit(0);
    return 0;
}

int cmd_shell_register_handle()
{
    cmd_shell_handle_func[0] = cmd_shell_handle_draw_line;
    cmd_shell_handle_func[1] = cmd_shell_handle_draw_circle;
    cmd_shell_handle_func[2] = cmd_shell_handle_new;
    cmd_shell_handle_func[3] = cmd_shell_handle_tag;
    cmd_shell_handle_func[4] = cmd_shell_handle_move;
    cmd_shell_handle_func[5] = cmd_shell_handle_resize;
    cmd_shell_handle_func[6] = cmd_shell_handle_statusc;
    cmd_shell_handle_func[7] = cmd_shell_handle_select;
    cmd_shell_handle_func[8] = cmd_shell_handle_draw_rect;
    cmd_shell_handle_func[9] = cmd_shell_handle_rot;
    cmd_shell_handle_func[10] = cmd_shell_handle_del;
    cmd_shell_handle_func[11] = cmd_shell_handle_draw_eclipse;
    cmd_shell_handle_func[12] = cmd_shell_handle_copy;
    cmd_shell_handle_func[13] = cmd_shell_handle_draw_arc;

    //命令解释器指令集
    cmd_shell_handle_func[14] = cmd_shell_handle_use;
    cmd_shell_handle_func[15] = cmd_shell_handle_rfile;
    cmd_shell_handle_func[16] = cmd_shell_handle_create;
    cmd_shell_handle_func[17] = cmd_shell_handle_record;
    cmd_shell_handle_func[18] = cmd_shell_handle_recordstop;
    cmd_shell_handle_func[19] = cmd_shell_handle_exit;

    cmd_shell_handle_func_count = 20;
    return 0;
}