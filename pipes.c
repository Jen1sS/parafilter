#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void send(char* msg,int size, int fd){
    int toSend = strlen(msg);
    int sent = 0;

    while (toSend>=0){        
        write(fd,&msg[sent],size);
        toSend-=size;
        sent+=size;
    }

    //write(fd,"\n",size);
}