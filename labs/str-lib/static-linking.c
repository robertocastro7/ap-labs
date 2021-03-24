#include <stdio.h>
#include <string.h>

int mystrlen(char *str);
char *mystradd(char *origin, char *addition);
int mystrfind(char *origin, char *substr);

int main(int argc, char **argv) {
    if (argc == 4){        
        char *command=argv[1];
        char *str1 =argv[2];
        char *str2=argv[3];
        if(strcmp(command,"-add") == 0){
            mystradd(str1,str2);
        }else if(strcmp(command,"-find") == 0){
            mystrfind(str1,str2);
        }
    }else{
        printf("Arguments wrong\n");
        return 0;    
    }    
    return 0;
}