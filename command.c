//命令解释器：实现

#include <stdio.h>
#include <string.h>

#include "config.h"
#include "cmdcontrol.h" //钩子函数库
#include "command.h"

static int command_id_count = 0;

//提示：全局变量注意使用！！！！！

struct command * command_object_buffer[__CMD_SHELL_COMMAND_COUNT__];  //命令解释器对象容器
int command_object_buffer_count       = 0;  //命令解释器对象数量
struct command * command_object_inuse = NULL;  //当前使用的命令解释器指针

//-------------------------------------------------------------------------------------------

static int __cmd_shell_funtion_call(struct command *cthis,int argc,char *argv[])
{
    //调用命令处理函数
    for(int i = 0; i < cmd_shell_handle_func_count ; i++)
        if(cmd_shell_handle_func[i](cthis,argc,argv) == 0)  //有函数应答
            return 0;
    return -1; //无函数应答
}

//显示欢迎信息
static int __cmd_shell_welcome(struct command *cthis)
{
    if(__CFG_IS_QUITE__)  //机读格式
        return 0;

    printf("Simple CAD System Ver. %s %s \n",__VER__,__PLATFORM__);
    return 0;
}

//------------------------------------------------------------------------------------------

//工厂函数：读入文件的Command工厂函数
struct command * factory_command_file(const char *file)
{
    FILE * fp_read  = fopen(file,"r");
    FILE * fp_write = fopen("/dev/null","w"); //指向空流
    FILE * fp_aux   = fopen("/dev/null","w"); //空流

    if(!(fp_read && fp_write && fp_aux))
        //读取失败
        return NULL;

    struct command *cthis = command_new(fp_read,fp_write,fp_aux);

    return cthis;
}

struct command * command_new(FILE *stream_in,FILE *stream_out,FILE *stream_aux)
{
    struct command *cthis = malloc(sizeof(struct command));
    if(!cthis)
        return NULL;

    cthis->id = command_id_count++;
    cthis->stream_in = stream_in;
    cthis->stream_out = stream_out;
    cthis->stream_aux = stream_aux;

    return cthis;
}

//删除命令解释器
int command_delete(struct command *cthis)
{
    free(cthis);
    return 0;
}

//标记使用
int command_use(int id)
{
    for(int i = 0; i < command_object_buffer_count; i++)
        if(command_object_buffer[i]->id == id) {
            command_object_inuse = command_object_buffer[i];
            return 0;
        }
    return -1;
}

//在队列中删除命令解释器
int command_object_delete(struct command *object)
{
    int delete_id = -1;

    for(int i = 0; i < command_object_buffer_count ; i++)
        if(command_object_buffer[i] == object)
        {
            delete_id = i;
            break;
        }

    if(delete_id == -1 )
        //未找到删除的对象：指针错误
        return -1;

    command_delete(command_object_buffer[delete_id]);

    //从队列中删除对象指针
    for(int i = delete_id; i < command_object_buffer_count - 1; i++)
        command_object_buffer[i] = command_object_buffer[i+1];
    
    command_object_buffer_count--; // 减少计数

    return 0;
}
//在队列中添加新的命令解释器
int command_object_insert(struct command *object)
{
    command_object_buffer[command_object_buffer_count] = object;
    command_object_buffer_count++;

    return 0; 
}

//显示错误信息
int cmd_shell_error(struct command *cthis,int errno, char *msg)
{
    fprintf(cthis->stream_out,"(%d)%s\n",errno,msg);
    return 0;
}

//命令解释器：行解析
int cmd_shell_phrase(struct command * cmd)
{
    //行缓冲区
    char line_buf[__CMD_SHELL_LINE_MAX__];

    //行参数分割
    char *argv_buf[__CMD_SHELL_ARGV_MAX__];
    memset(argv_buf,0,__CMD_SHELL_ARGV_MAX__);

    if(feof(cmd->stream_in))
        return 1; //输入流终止

    if(!__CFG_IS_QUITE__)
        fprintf(cmd->stream_out,"WORK %d>",command_object_inuse->id);  //提示符

    fgets(line_buf,__CMD_SHELL_LINE_MAX__,cmd->stream_in); //读取行

    if(line_buf[0] == '\n') //空行
        return 0;

    //去除\n
    for(int i = 0; line_buf[i] != 0; i++)
        if(line_buf[i] == '\n')
            line_buf[i] = 0;

    //分割
    argv_buf[0] = strtok(line_buf," ");
    int i = 1;
    int count = 1;
    for(i = 1;(argv_buf[i] = strtok(NULL," ")) != 0;i++)
        count ++;
    int argc = count; char **argv = argv_buf;

    //若辅助流开，复制输入到输出
    if(cmd->stream_aux)
        fprintf(cmd->stream_aux,"%s\n",line_buf);

    if(__cmd_shell_funtion_call(cmd,argc,argv))
        //执行失败：未找到命令
        cmd_shell_error(cmd,0,"Cannot find command.");

    return 0;
}

//命令解释器仲裁器入口：在每一条命令下处理命令解释器仲裁(类似于协同式操作系统)
int run_command_loop()
{
    cmd_shell_register_handle(); //注册命令处理钩子函数

    //显示欢迎画面
    __cmd_shell_welcome(command_object_inuse);

    while(1) {

        int cmd_phrase_result = cmd_shell_phrase(command_object_inuse);
        if(cmd_shell_phrase) // 当前使用的命令处理器流终止
            ; //TODO:命令处理器销毁
    }

    return 0;
}

//进入命令解释器仲裁循环前的准备工作
int run_command_prepare()
{
    struct command * init_cmd = command_new(stdin,stdout,NULL);   //进入运行循环的默认解释器
    command_object_insert(init_cmd); //进入调度队列
    command_use(0);           //0号命令解释器就需

    run_command_loop(); //进入命令解释器仲裁调度循环

    return 0; //函数永远不会返回
}