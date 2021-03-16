#include "main.h"
#include "getandanaOrder.h"
#include "execommand.h"
int main(int argc, char **argv)
{
    int i;
    int wordcount = 0;
    char wordmatrix[100][256];
    char **arg = NULL;
    char *buf = NULL; //用户输入

    if((buf = (char *)malloc(256))== NULL){
        perror("malloc failed");
        exit(-1);
    }

    while(1){
        memset(buf, 0, 256);    //将buf所指的空间清零
        message = (char *)malloc(100);
        getcwd(message, 100);
        printf("os_shell:%s # ", message);
        free(message);
        getOrder(buf);
        strcpy(msg[whilecnt],buf);
        whilecnt++;
        if( strcmp(buf, "exit\n") == 0 ){
            printf("Welcome to use my shell again!\n");
            break;
        }
        for(i = 0; i < 100; i++){
            wordmatrix[i][0] = '\0';
        }
        wordcount = 0;
        anaOrder(buf, &wordcount, wordmatrix);
        exeCommand(wordcount, wordmatrix);
    }

    if(buf != NULL){
        free(buf);
        buf = NULL;
    }
    return 0;
}
