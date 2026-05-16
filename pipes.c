
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "strings.h"

//Send a string without caring for buffer sizes
void send(char* msg,int size, int* fd){
    int toSend = strlen(msg);
    int sent = 0;

    msg[toSend-1]='\n';

    while (sent<toSend) 
        sent += write(fd[1],&msg[sent],size>toSend?toSend:size);
}

//Read a string without caring for buffer sizes
void readd(char** dest, int size, int* fd){
    char* inbuf= malloc(size + 1);
    int nbytes;

    while ((nbytes = read(fd[0], inbuf, size)) > 0) {
        inbuf[nbytes] = '\0'; 
        resize_and_strcat(dest,inbuf);
    }
}