#pragma once

#define __VER__ "1.0.0"
#define __AUTHOR__ "Lu Qiuwen"
#define __PLATFORM__ "x86-64 Linux"

#define __CFG_IS_QUITE__  0

//Cmd中最大的钩子函数数量
#define __CMD_SHELL_MAX_HANDLE_COUNT__ 50
//命令行最大长度
#define __CMD_SHELL_LINE_MAX__ 500
//命令行参数的个数         
#define __CMD_SHELL_ARGV_MAX__ 500         
//画布对象的默认最大值（倍增值）
#define __DRAWAREA_GRAPH_OBJ_DEFAULT_MAX__ 200

#define __GRAPH_DESCRIPTION_NAME_MAX__ 100

#define __DRAWAREA_STATUSBAR_INFOTEXT_MAX__ 100

#define __CMD_SHELL_COMMAND_COUNT__ 50


//=============图像标志参数==========================================

#define __TAG_FONT_SCALE__ 0.8
#define __TAG_FONT_COLOR__ CV_RGB(255,255,255)