#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void resize_and_strcatM(char** line, char** partial, int partial_size);


void openFile(FILE** file, char* source,char* mode){
    *file = fopen(source, mode); // Opening the file in read mode

    if (*file == NULL) { //is file opened
        char* err = "[";
        char* tmp[2] = {source,"] Error"};

        resize_and_strcatM(&err,tmp,2);

        perror(err);
    }
}
