
//主程序：环境准备

#include <stdio.h>
#include <stdlib.h>
#include "window.h"
#include "point.h"
#include "line.h"

#include "command.h"


int main(int argc, char const *argv[])
{

    run_command_prepare(); //命令解释器准备
    return 0;
}