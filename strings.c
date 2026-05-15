#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void resize_and_strcat(char** line, char* partial) {
    char* tmp = *line;

    *line = malloc(strlen(tmp) + strlen(partial) + 1);
    (*line)[0] = '\0';

    strcat(*line, tmp);
    strcat(*line, partial);

    fflush(stdout);
    //free(tmp);
}

void resize_and_strcatM(char** line, char** partial, int partial_size) {
    char* tmp = *line;
    int size=0;

    for (int i = 0; i < partial_size; i++)
        size+=strlen(partial[i]);
    
    *line = malloc(size+1);
    (*line)[0] = '\0';

    strcat(*line, tmp);
    for (int i = 0; i < partial_size; i++)
    {
        strcat(*line, partial[i]);
    }

    fflush(stdout);
    //free(tmp);
}

