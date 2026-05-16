#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

//test 1: parafilter -e errori.txt "root" /etc/shadow /etc/passwd
//test 2: parafilter "password" /var/log/syslog /var/log/auth.log

short search(int data_size, char* data, int keyword_size, char* keyword);
void resize_and_strcat(char** line, char* partial);
void resize_and_strcatM(char** line, char** partial, int partial_size);
void send(char* msg,int size,int fd);
void openFile(FILE** file, char* source,char* mode);

int main(int argc, char **argv)
{

    //both related
    unsigned short sizeDef = 50; //default size for reading from file and writing in pipe
    int fid = getpid(); //father id

    //father related
    unsigned short errors = 0; //error reporting
    char* inbuf = malloc(sizeDef + 1);
    int nbytes;

    //children related
    char* keyword; //keyword
    int keyword_size;
    char* childsource; //file source for child
    int start; //variable start for arguments since there can be -e file_name
    int children_size; //n of children needed
    int fd[2]; //pipe management
    char* line = "";
    char partial[sizeDef];

    //args check
    if (argc<3) {
        printf("Error: too few arguments passed.");
        return 1;
    }

    //check if user wants eror reporting
    if (argv[1][0]=='-' && argv[1][1]=='e') { 
        errors++;
        keyword = argv[3];

        //dup2 to redirect stderr
        int errdesc = open(argv[2],O_WRONLY | O_APPEND | O_CREAT, 0600);

        if (errdesc < 0) {
            perror("Error logs error");
            return 1;
        }
        dup2(errdesc,2);


    } else keyword = argv[1];
    
    //fetch keyword size
    keyword_size = strlen(keyword);

    //child creation and path allocation
    start = 1-2*errors; 
    children_size = argc-start;
    pid_t children[children_size]; 
    pipe(fd);

    for (int i=2+2*errors; i<argc; i++) {
        children[i-start]=fork();

        if(children[i-start]<0) {
            perror("fork fail");
            exit(1);
        } else if ( getpid() != fid ) {
            close(fd[0]);
            childsource = argv[i];
            i=argc;
        }
    }

    //children code
    if (getpid()!=fid) {  

        FILE* fptr;
        openFile(&fptr,childsource,"r");

        line = "";

        // Read the data from the file using fgets() method
        while (fgets(partial, sizeDef, fptr) != NULL)
        {
            int ind=-1;

            for (int i=0; i<sizeDef; i++) 
                if (partial[i]==0 || partial[i]==10) {
                    ind=i+1;
                    i=sizeDef;
                }

            resize_and_strcat(&line, partial);
                
            if (ind!=-1 && ind!=sizeDef) {
                                        
                if (search(strlen(line),line,keyword_size,keyword)) {
                    
                    char* tmp[3] = {childsource,"]:",line};
                    char* out = "[";

                    resize_and_strcatM(&out,tmp,3);

                    //printf("Original (%d): %s\n",strlen(out),out);
                    send(out,sizeDef,fd[1]);

                    fflush(stdout);

                }

                line = malloc(sizeof(char*));
                line = "";
            }

        }
        
        fclose(fptr); // Closing the file 
        return 0;
    }
    
    //father code
    close(fd[1]);

    while ((nbytes = read(fd[0], inbuf, sizeDef)) > 0) {
        inbuf[nbytes] = '\0';
        fprintf(stdout,"%s",inbuf);
    }

    return 0;
}

