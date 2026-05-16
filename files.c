#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#include "strings.h"


int openFile(FILE** file, char* source,char* mode){
    *file = fopen(source, mode); // Opening the file in read mode

    if (*file == NULL) { //is file opened
        return errno;
    }
    return 0;
}


//moves stderr, stdout, stdin to a specified file
int moveSTD(char* newstd,int type){
    //dup2 to redirect stderr
    int errdesc = open(newstd,O_WRONLY | O_APPEND | O_CREAT, 0600);

    if (errdesc < 0) {
        return errno;
    }
    
    dup2(errdesc,type);
    return 0;
}
