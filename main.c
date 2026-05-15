#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

//test 1: parafilter -e errori.txt "root" /etc/shadow /etc/passwd
//test 2: parafilter "password" /var/log/syslog /var/log/auth.log

short search(int data_size, char* data, int keyword_size, char* keyword);
void resize_and_strcat(char** line, char* partial);
void resize_and_strcatM(char** line, char** partial, int partial_size);
void send(char* msg,int size,int fd);

int main(int argc, char **argv)
{

    unsigned short sizeDef = 10; //size default for reading from file and writing in pipe

    int fid = getpid(); //father id

    unsigned short errors = 0; //error reporting
    char* efile; //error file

    char* keyword; //keywordmk
    int keyword_size;

    char* childsource; //file source for child

    if (argc<3) {
        printf("Error: too few arguments passed.");
        return 1;
    }

    if (argv[1][0]=='-' && argv[1][1]=='e') {
            errors++;
            //efile = argv[2];
            keyword = argv[3];
    } else {
        keyword = argv[1];
    }
    keyword_size = strlen(keyword);

    int start = 1-2*errors; //variable start for arguments since there can be -e file_name
    int children_size = argc-start; //n of children needed
    pid_t children[children_size]; 
    int fd[2];
    pipe(fd);

    for (int i=2+2*errors; i<argc; i++) { //child creation and path allocation
        children[i-start]=fork();

        if(children[i-start]<0) {
            //perror("fork fail");
            exit(1);
        } else if ( getpid() != fid ) {
            close(fd[0]);
            childsource = argv[i];
            i=argc;
        }
    }

    if (getpid()!=fid) {

        FILE* fptr;
        fptr = fopen(childsource, "r"); // Opening the file in read mode

        if (fptr == NULL) { //is file opened
            char* err = "[";
            char* tmp[2] = {childsource,"] Error"};

            resize_and_strcatM(&err,tmp,2);

            //perror(err);
            fflush(stdout);
            exit(1);
        }
            //printf("[%s]: Opened successfully\n",childsource);
            char* line = "";

            char partial[sizeDef];

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
    
    char* inbuf = malloc('a'*sizeDef);
    int nbytes;

    close(fd[1]);

    while ((nbytes = read(fd[0], inbuf, sizeDef)) > 0) 
            printf("%s",inbuf);

    return 0;
}

