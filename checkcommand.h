//
// Created by  JasonXu on 2021/3/15.
//

#ifndef SHELL_CHECKCOMMAND_H
#define SHELL_CHECKCOMMAND_H

int checkCommand(char *command){
    DIR *dp;
    struct dirent *dirp;
    char *path[] = {"./", "/bin", "/usr/bin", NULL};

    if( strncmp(command, "./", 2) == 0 )
    {
        command = command + 2;
    }


    int i = 0;
    while(path[i] != NULL)
    {
        if( (dp= opendir(path[i])) ==NULL )
        {
            printf("can not open /bin \n");
        }
        while( (dirp = readdir(dp)) != NULL )
        {
            if(strcmp(dirp->d_name, command) == 0)
            {
                closedir(dp);
                return 1;
            }
        }
        closedir(dp);
        i++;
    }
    return 0;
}



#endif //SHELL_CHECKCOMMAND_H
