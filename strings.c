#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Function to add something to the end of a string and not care about resizing, for multiple strings
void resize_and_strcatM(char** line, char** partial, int partial_size) {
    char* tmp = *line;
    int size=strlen(tmp);

    for (int i = 0; i < partial_size; i++){
        size+=strlen(partial[i]);
    }    

    *line = malloc(size+1);
    (*line)[0] = '\0';

    strcat(*line, tmp);
    for (int i = 0; i < partial_size; i++)
    {
        strcat(*line, partial[i]);
    }
}


//Function to add something to the end of a string and not care about resizing
void resize_and_strcat(char** line, char* partial) {
    resize_and_strcatM(line,&partial, 1);
}
