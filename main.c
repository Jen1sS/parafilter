#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>


#include "files.h"
#include "pipes.h"
#include "search.h"
#include "strings.h"

//test 1: parafilter -e error.log cipher ./test.txt ./test2.txt ./test3.txt ./gurtyo.txt ./gia/ciao.txt   
//test 2: parafilter cipher ./test.txt ./test2.txt ./test3.txt ./gurtyo.txt ./gia/ciao.txt
//test 3: parafilter

int main(int argc, char **argv)
{

    //both related
    unsigned short sizeDef = 100; //default size for reading from file and writing in pipe
    int fid = getpid(); //father id
    int id;


    //father related
    unsigned short errors = 0; //error reporting
    char* recieved = "";

    //children related
    char* keyword; //keyword
    int keyword_size;
    char* childsource; //file source for child
    int start; //variable start for arguments since there can be -e file_name
    int children_size; //n of children needed
    char* line = "";
    char partial[sizeDef];
    int pipeID;

    //args check
    if (argc<3) {
        printf("Error: too few arguments passed.");
        return 1;
    }

    //check if user wants eror reporting
    if (argv[1][0]=='-' && argv[1][1]=='e') { 
        errors++;
        keyword = argv[3];

        if (moveSTD(argv[2],2)!=0) {
            perror("Error during dup2 call:");
            return 1;
        }

    } else keyword = argv[1];
    
    //fetch keyword size
    keyword_size = strlen(keyword);

    //#region child creation and path allocation
    start = 2+2*errors; 
    children_size = argc-start;

    pid_t children[children_size]; 
    int fd[children_size][2]; //pipe management

    for (int i=0; i<children_size; i++) {
        if (pipe(fd[i])==-1) {
            perror("Error creating pipe");
            exit(1);
        }

        children[i]=fork();

        id = getpid()-fid;

        if(children[i]<0) {
            perror("fork fail");
            exit(1);
        } else if (id !=0) {
            pipeID = i;
            close(fd[i][0]);
            childsource = argv[i+start];
            i=argc;
        } else if (id==0) close(fd[i][1]);
    }


    //children code
    if (id !=0) {  

        FILE* fptr;
        if (openFile(&fptr,childsource,"r")!=0) {

            char* err = "[";
            char* tmp[2] = {childsource,"] Error"};

            resize_and_strcatM(&err,tmp,2);
            perror(err);
            exit(1);
        }

        line = "";

        // Read the data from the file using fgets() method
        while (fgets(partial, sizeDef, fptr) != NULL)
        {
            int ind=-1;

            //TODO: Optimize
            for (int i=0; i<sizeDef; i++) 
                if (partial[i]==0 || partial[i]==10) {
                    ind=i+1;
                    i=sizeDef;
                }

            resize_and_strcat(&line, partial);
                
            if (ind!=-1 && ind!=sizeDef) {
                     
                //TODO: Optimize
                if (search(strlen(line),line,keyword_size,keyword)) {
                    
                    char* tmp[3] = {childsource,"]:",line};
                    char* out = "[";

                    resize_and_strcatM(&out,tmp,3);
                    send(out,sizeDef,fd[pipeID]);
                }

                line = malloc(sizeof(char*));
                line = "";
            }

        }
        
        fclose(fptr); // Closing the file 
        exit(0);
    }
    
    //father code
    int end = 0;
    int status;

    while (end<children_size) {
        for (int i = 0; i < children_size; i++)
        {
            //waitpid(children[i],&status,WNOHANG);
            if (waitpid(children[i],&status,WNOHANG)!=0) end++;
            else end==0;

            readd(&recieved,sizeDef,fd[i]);
            fprintf(stdout,"%s",recieved);
            recieved="";
        }
    }
    return 0;
}

