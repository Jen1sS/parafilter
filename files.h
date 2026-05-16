#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int openFile(FILE** file, char* source,char* mode);

int moveSTD(char* source,int type);