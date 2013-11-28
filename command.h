//命令解释器头文件

//命令解释器欢迎
#pragma once
#include "window.h"
#include "drawarea.h"

struct command { //命令解释器

    int id;      //命令解释器序号
    struct window * window;
    struct drawarea * drawarea;

    FILE *stream_in;    //输入流
    FILE *stream_out;   //输出流
    FILE *stream_aux;   //辅助流（复制流）
};


struct command * command_new(FILE *stream_in,FILE *stream_out,FILE *stream_aux);
int command_delete(struct command *cthis);
int command_use(int id);
int command_object_delete(struct command *cthis);
int command_object_insert(struct command *cthis);
int cmd_shell_error(struct command *cthis,int errno, char *msg);
int cmd_shell_phrase(struct command * cmd);

struct command * factory_command_file(const char *file);


int run_command_loop();
int run_command_prepare();