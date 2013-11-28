
//Command 函数处理库头文件

#pragma once

#include "command.h"

extern int (*cmd_shell_handle_func[__CMD_SHELL_MAX_HANDLE_COUNT__])(struct command *cthis,int argc,char *argv[]);
//函数钩子

extern int cmd_shell_handle_func_count;
//函数钩子数量

#define HANDLE_RESPONSE(cmd) do { if(strcmp(cmd,argv[0]) != 0) return -1;} while(0)

#define GET_INPUT(var,type,dest)                                            \
do {                                                                        \
    int result = sscanf(var,type,&(dest));                                  \
    if(result == 0)                                                         \
    {                                                                       \
        cmd_shell_error(cthis,5,"input error");                             \
        return 0;                                                           \
    }} while(0)

#define CHECK_PNUM(num)                                                     \
    do{                                                                     \
        if(argc != num+1)                                                   \
        {                                                                   \
            cmd_shell_error(cthis,6,"input count error");                   \
            return 0;                                                       \
        }}while(0)                                                         


int cmd_shell_register_handle();