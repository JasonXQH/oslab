//
// Created by  JasonXu on 2021/3/15.
//

#ifndef SHELL_GETANDANAORDER_H
#define SHELL_GETANDANAORDER_H

void getOrder(char *buf){
    int cnt = 0;

    int c = getchar();
    while(cnt < 256 && c != '\n'){
        buf[cnt++] = c;
        c = getchar();
    }
    buf[cnt++]='\n';
    buf[cnt]='\0';
    if(cnt == 256){
        exit(-1);
    }
}


void anaOrder(char* buf, int* wordcount, char (*wordmatrix)[256]){
    char *p = buf;
    char *q = buf;
    int number = 0;
    int i;
    while(1){
        if(p[0] == '\n'){
            break;
        }
        if(p[0] == ' '){
            p++;
        }
        else{
            q = p;
            number = 0;
            while((q[0] != ' ') && (q[0] != '\n')){
                number++;
                q++;
            }
            strncpy(wordmatrix[*wordcount], p, number + 1);
            wordmatrix[*wordcount][number] = '\0';
            *wordcount = *wordcount + 1;
            p = q;
        }
    }
}


#endif //SHELL_GETANDANAORDER_H
