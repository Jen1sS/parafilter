#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void resize_and_strcat(char** line, int partial_size, char* partial) {
    char* tmp = *line;

    *line = malloc(strlen(tmp) + strlen(partial) + 1);
    (*line)[0] = '\0';

    strcat(*line, tmp);
    strcat(*line, partial);

    fflush(stdout);
    //free(tmp);
}

short search(int data_size, char* data, int keyword_size, char* keyword){
    int match = 0;      

    if (keyword[0]==data[0]) match=1;

    for (int i=1; i<data_size+1; i++) {
        if (match>=keyword_size && (data[i]==' ' || data[i]=='\n' || data[i]==0)) return 1;  

        if ( match>-1 && keyword[match]==data[i]) match++;
        else if (data[i] == ' ') match=0;
        else match=-1;

    }
    return 0;
}