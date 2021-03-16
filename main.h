//
// Created by  JasonXu on 2021/3/15.
//

#ifndef SHELL_MAIN_H
#define SHELL_MAIN_H
#include <stdio.h>//标准I/O库
#include <stdlib.h>//实用程序库函数
#include <string.h>//字符串操作
#include <signal.h>// 信号
#include <curses.h>
#include <limits.h>//实现常量
#include <termcap.h>
#include <termios.h>
#include <time.h>//时间和日期
#include <errno.h>//出错码
#include <assert.h>//验证程序断言

#include <dirent.h>//目录项
#include <fcntl.h>//文件控制
#include <pwd.h>//口令文件
#include <unistd.h>//符号常量

#include <minix/com.h>
#include <minix/config.h>
#include <minix/type.h>
#include <minix/endpoint.h>
#include <minix/const.h>
#include <minix/u64.h>
#include <paths.h>
#include <minix/procfs.h>

#include <sys/stat.h>//文件状态
#include <sys/types.h>//基本系统数据类型
#include <sys/wait.h>//进程控制
#include <sys/times.h>//进程时间
#include <sys/time.h>
#include <sys/select.h>//select函数


#define _PATH_PROC "/proc/"
#define PSINFO_VERSION	0
#define TYPE_TASK	'T'
#define TYPE_SYSTEM	'S'
#define TYPE_USER	'U'
#define STATE_RUN	'R'
#define TIMECYCLEKEY 't'
#define ORDERKEY 'o'

#define  USED		0x1
#define  IS_TASK	0x2
#define  IS_SYSTEM	0x4
#define  BLOCKED	0x8

#define ORDER_CPU	0
#define ORDER_MEMORY	1


const char *cputimenames[] = { "user", "ipc", "kernelcall" };
#define CPUTIMENAMES (sizeof(cputimenames)/sizeof(cputimenames[0]))


struct proc {
    int p_flags;//
    endpoint_t p_endpoint;//端点
    pid_t p_pid;//进程号
    u64_t p_cpucycles[CPUTIMENAMES];//cpu周期
    int p_priority; //动态优先级
    endpoint_t p_blocked; //阻塞状态
    time_t p_user_time; //用户时间
    vir_bytes p_memory; //内存
    uid_t p_effuid;//有效用户ID
    int p_nice; //静态优先级
    char p_name[PROC_NAME_LEN+1]; //名字
};

struct proc *proc = NULL, *prev_proc = NULL;

struct tp {
    struct proc *p;
    u64_t ticks;
};

void getOrder(char *);     //得到输入的命令
void anaOrder(char *, int *, char (*)[256]);  //对输入的命令进行解析
void exeCommand(int, char (*)[256]);   //执行命令
int checkCommand(char *command);   //查找命令中的可执行程序
void getkinfo();//读取/proc/kinfo得到总的进程和任务数nr_total
int print_memory();//输出内存信息
void get_procs(); //记录当前进程，赋值给prev_proc
void parse_dir();//获取到/proc/下的所有进程pid
void parse_file(pid_t pid);//获取每一个进程信息
u64_t cputicks(struct proc *p1, struct proc *p2, int timemode);
void print_procs(struct proc *proc1, struct proc *proc2, int cputimemode);//输出CPU使用时间

char *message;  //用于myshell提示信息的输出
int whilecnt=0;
char msg[40][256];
unsigned int nr_procs, nr_tasks;//进程数和任务数
int nr_total;//进程和任务总数
int order = ORDER_CPU;
int blockedverbose = 0;
char *Tclr_all;
int slot=1;

#endif //SHELL_MAIN_H
