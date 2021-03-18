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

    for(i = 0; i < wordcount; i++){
        word[i] = (char *)wordmatrix[i];
    }
    word[wordcount] = NULL;

    void set_way(char *word[],int way,int correct,int i);

    for(i = 0; i < wordcount; i++){
        if(strncmp(word[i], "&", 1) == 0){
            way=5;
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
        if(strcmp(word[i], ">>") == 0){
            way = 4;
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
    if(correct==0){
        printf("wrong format\n");
        return ;
    }

    if(way == 1){
        for(i = 0; word[i] != NULL; i++){
            if(strcmp(word[i], ">") == 0){
                file = word[i+1];
                word[i] = NULL;
            }
        }
    }

    if(way == 2){
        for(i = 0; word[i] != NULL; i++){
            if(strcmp(word[i], "<") == 0){
                file = word[i + 1];
                word[i] = NULL;
            }
        }
    }

    if(way == 3){
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
    if(way == 4){
        for(i = 0; word[i] != NULL; i++){
            if(strcmp(word[i], ">>") == 0){
                file = word[i+1];
                word[i] = NULL;
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
    if((pid = fork()) < 0){
        printf("fork error\n");
        return ;
    }
    if(strcmp(word[0],"history")==0){
        if(pid==0){
            char *cnum = word[1];
            int num = atoi(cnum);
            printf("%d", num);
            for (int i = whilecnt - 1; i > whilecnt - num - 1; i--) {
                printf("command %d:", i);
                printf("%s", msg[i]);
            }
            exit(0);
        }
    }

    if(strcmp(word[0], "mytop") == 0){
        if(pid==0){
            int cputimemode = 1;
            getkinfo();
            print_memory();
            get_procs();
            if (prev_proc == NULL)
                get_procs();
            print_procs(prev_proc, proc, cputimemode);
            exit(0);
        }
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
            if(pid == 0){
                if( !(checkCommand (word[0])) ){
                    printf("%s : command not found\n", word[0]);
                    exit(0);
                }
                fd = open(file, O_RDONLY);
                dup2(fd, 0);
                execvp(word[0], word);
            }
            break;

        case 3:
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
        case 4:
            if(pid == 0){
                if( !(checkCommand(word[0])) ){
                    printf("%s : command not found\n", word[0]);
                    exit(0);
                }
                fd = open(file, O_RDWR|O_CREAT|O_APPEND, 0644);
                dup2(fd, 1);
                execvp(word[0], word);
                exit(0);
            }
            break;
        case 5:
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
            }
            break;
        default:
            break;
    }

    if(waitpid(pid, &status, 0) == -1){
        printf("wait for child process error\n");
    }
}
#endif
