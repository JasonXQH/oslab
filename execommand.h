//
// Created by  JasonXu on 2021/3/15.
//

#ifndef SHELL_EXECOMMAND_H
#define SHELL_EXECOMMAND_H
#include "main.h"
#include "mytop.h"
#include "checkcommand.h"
void exeCommand(int wordcount, char (*wordmatrix)[256]){
    pid_t pid;
    char *file;
    int correct = 1;
    int way = 0;
    int status;
    int i;
    int fd;
    char *word[wordcount + 1];
    char *wordnext[wordcount + 1];

    //将命令取出
    for(i = 0; i < wordcount; i++){
        word[i] = (char *)wordmatrix[i];
    }
    word[wordcount] = NULL;

    //查看命令行是否有后台运行符
    for(i = 0; i < wordcount; i++){
        if(strncmp(word[i], "&", 1) == 0){
            way=4;
            word[wordcount - 1] = NULL;
        }
    }

    for(i = 0; word[i] != NULL; i++){
        if(strcmp(word[i], ">") == 0){
            way = 1;
            if(word[i + 1] == NULL){
                correct=0;
            }
        }
        if(strcmp(word[i], "<") == 0){
            way = 2;
            if(i == 0){
                correct=0;
            }
        }
        if(strcmp(word[i], "|") == 0){
            way = 3;
            if(word[i+1] == NULL){
                correct=0;
            }
            if(i == 0){
                correct=0;
            }
        }
    }
    //correct=0,说明命令格式不对
    if(correct==0){
        printf("wrong format\n");
        return ;
    }

    if(way == 1){
        //输出重定向
        for(i = 0; word[i] != NULL; i++){
            if(strcmp(word[i], ">") == 0){
                file = word[i+1];
                word[i] = NULL;
            }
        }
    }

    if(way == 2){
        //输入重定向
        for(i = 0; word[i] != NULL; i++){
            if(strcmp(word[i], "<") == 0){
                file = word[i + 1];
                word[i] = NULL;
            }
        }
    }

    if(way == 3){
        //把管道符后面的部分存入wordnext中，管道后面的部分是一个可执行的shell命令
        for(i = 0; word[i] != NULL; i++){
            if(strcmp(word[i], "|") == 0){
                word[i] = NULL;
                int j;
                for(j = i + 1; word[j] != NULL; j++){
                    wordnext[j-i-1] = word[j];
                }
                wordnext[j-i-1] = NULL;
            }
        }
    }

    if(strcmp(word[0], "cd") == 0){
        if(word[1] == NULL){
            return ;
        }
        if(chdir(word[1]) ==  -1){
            perror("cd");
        }
        return ;
    }

    if(strcmp(word[0], "history") == 0){  //实现history n
            char cnum=word[1][0];
            int num=cnum-48;
            for(int i=whilecnt-1;i>whilecnt-num-1;i--){
                printf("command %d:",i);
                printf("%s",msg[i]);
            }

    }

    if(strcmp(word[0], "mytop") == 0){
            int cputimemode = 1;
            getkinfo();
            print_memory();
            get_procs();
            if (prev_proc == NULL)
                get_procs();
            print_procs(prev_proc, proc, cputimemode);
    }

    if((pid = fork()) < 0){
        printf("fork error\n");
        return ;
    }
    switch(way){
        case 0:
            if(pid == 0){
                if(!(checkCommand(word[0]))){
                    printf("%s : command not found\n", word[0]);
                    exit(0);
                }
                execvp(word[0], word);
                exit(0);
            }
            break;

        case 1:
            //输入的命令中含有输出重定向符
            if(pid == 0){
                if( !(checkCommand(word[0])) ){
                    printf("%s : command not found\n", word[0]);
                    exit(0);
                }
                fd = open(file, O_RDWR|O_CREAT|O_TRUNC, 0644);
                dup2(fd, 1);
                execvp(word[0], word);
                exit(0);
            }
            break;

        case 2:
            //输入的命令中含有输入重定向<
            if(pid == 0){
                if( !(checkCommand (word[0])) ){
                    printf("%s : command not found\n", word[0]);
                    exit(0);
                }
                fd = open(file, O_RDONLY);
                dup2(fd, 0);
                execvp(word[0], word);
                exit(0);
            }
            break;

        case 3:
            //输入的命令中含有管道符|
            if(pid == 0){
                int pid2;
                int status2;
                int fd[2];

                if(pipe(fd)<0){
                    printf("pipe error\n");
                    exit(0);
                }

                if( (pid2 = fork()) < 0 ){
                    printf("fork2 error\n");
                    return ;
                }
                else if(pid2 == 0){
                    if( !(checkCommand(word[0])) ){
                        printf("%s : command not found\n", word[0]);
                        exit(0);
                    }
                    dup2(fd[1], 1);
                    execvp(word[0], word);
                    exit(0);
                }
                if(waitpid(pid2, &status2, 0) == -1){
                    printf("wait for child process error\n");
                }
                if( !(checkCommand(wordnext[0])) ){
                    printf("%s : command not found\n", wordnext[0]);
                    exit(0);
                }
                dup2(fd[0], 0);
                execvp (wordnext[0], wordnext);
                exit(0);
            }
            break;
        case 4://若命令中有&，表示后台执行，父进程直接返回，不等待子进程结束
            signal(SIGCHLD,SIG_IGN);
            if(pid==0){

                printf("[process id: %d]\n",pid);
                int a = open("/dev/null",O_RDONLY);
                dup2(a,0);
                dup2(a,1);
                if( !(checkCommand(word[0])) ){
                    printf("%s : command not found\n", word[0]);
                    exit(0);
                }
                execvp(word[0],word);
                exit(0);
                //}
            }
            break;
    }

    //父进程等待子进程结束
    if(waitpid(pid, &status, 0) == -1){
        printf("wait for child process error\n");
    }
}
#endif
