#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

//test 1: parafilter -e errori.txt "root" /etc/shadow /etc/passwd
//test 2: parafilter "password" /var/log/syslog /var/log/auth.log

short search(int data_size, char* data, int keyword_size, char* keyword);
void resize_and_strcat(char** line, int partial_size, char* partial);

int main(int argc, char **argv)
{
    int fid = getpid();
    unsigned short errors = 0;
    char* efile; //error file

    char* keyword;
    int keyword_size;

    char* childsource;

    if (argc<3) {
        printf("Error: too few arguments passed.");
        return 1;
    }

    if (argv[1][0]=='-' && argv[1][1]=='e') {
            errors++;
            efile = argv[2];
            keyword = argv[3];
    } else {
        keyword = argv[1];
    }
    keyword_size = strlen(keyword);

    int start = 1-2*errors;
    pid_t children[argc-start];

    for (int i=2+2*errors; i<argc; i++) { //child creation and path allocation
        children[i-start]=fork();

        if(children[i-start]<0)
        {
            perror("fork fail");
            exit(1);
        } else if ( getpid() != fid ) {
            childsource = argv[i];
            i=argc;
        }
    }

    if (getpid()!=fid) {

        FILE* fptr;
        fptr = fopen(childsource, "r"); // Opening the file in read mode

        if (fptr == NULL) { //is file opened
            char* err = malloc(sizeof("[] Error:")+sizeof(childsource)+1);
            err[0] = '\0'; // ensures the memory is an empty string

            strcat(err,"[");
            strcat(err,childsource);
            strcat(err,"] Error");

            perror(err);
            exit(1);
        }

            char* line = "";

            int partial_size = 100;
            char partial[partial_size];

            // Read the data from the file using fgets() method
            while (fgets(partial, partial_size, fptr) != NULL)
            {
                int ind=-1;

                for (int i=0; i<partial_size; i++) 
                    if (partial[i]==0 || partial[i]==10) {
                        ind=i+1;
                        i=partial_size;
                    }

                resize_and_strcat(&line, partial_size, partial);
                
                if (ind!=-1 && ind!=partial_size) {
                                        
                    if (search(strlen(line),line,keyword_size,keyword)) {
                       printf("[%s]:%s",childsource,line);
                       fflush(stdout);

                    }

                    line = malloc(sizeof(char*));
                    line = "";
                }

            }
        
            fclose(fptr); // Closing the file 
    }
    
    return 0;
}

