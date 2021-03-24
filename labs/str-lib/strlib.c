#include <stdio.h>   
#include <stdlib.h>


int mystrlen(char *str){
    int acum = 0;
    while (str[acum] != NULL){
        acum++;
    }
    return acum;
}

char *mystradd(char *origin, char *addition){
    printf("%s\t%d\n","Initial Length:",mystrlen(origin));
    
    int i = 0;
    int newSize = mystrlen(origin)+mystrlen(addition);
    char newCharacter[newSize];

    for (int i = 0; i < mystrlen(origin); i++){
        newCharacter[i] = origin[i];
    }
    for (int j = mystrlen(origin); j < (mystrlen(origin)+mystrlen(addition)); j++){
        i++;
        newCharacter[j] = addition[i];        
    }

    printf("New String: %s\n", newCharacter);
    printf("New Lenght: %d\n", mystrlen(newCharacter));

    return 0;
}

int mystrfind(char *origin, char *substr){
    printf("%s%s%s","['",substr,"']"); 
    int current = 0;
    while (*origin != '\0') {
        while(*origin ==* substr){
            origin++;
            substr++;
            if(*substr == '\0'){
                printf("%s String was found at [%d] position\n",substr,current); 
                return 0;
            }
        }
        substr = &substr[0];
        current++;
        origin++;
    }
    printf("%s\n","String was not found "); 
    return 0;
}