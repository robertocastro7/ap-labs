
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int size = 0;
int num = 0;
char *data[20000];

void getElem(char *repo, char *res);
void merge(void *data[], int l,int mid, int r, int (*comp)(void *, void *));
void mergeSort(void *data[], int l, int r, int (*comp)(void*, void*));
void print(char *res);
int ncmp(char *s1,char *s2);

int ncmp(char *s1, char *s2){
    double v1, v2;
    v1 = atof(s1);
    v2 = atof(s2);
    if(v1 < v2){
        return -1;
    }else if(v1 > v2){
        return 1;
    }else{
        return 0;
    }
}

void print(char *res){
    FILE *rep;
    rep = fopen(res, "w");
    for (int x = 0; x < size; x++){
        fprintf(rep, "%s", data[x]);
    }
    fclose(rep);
    printf("Results file can be found at [%s]\n", res);
}

void merge(void *data[], int l, int mid, int r, int (*comp)(void *, void *)){
    int numO = mid - l + 1;
    int numT = r - mid;
    void *L[numO];
    void *R[numT];
    for(int x = 0; x < numO; x++){
        L[x] = data[l + x];
    }
    for(int x = 0; x < numT; x++){
        R[x] = data[mid + 1 + x];
    }
    int x = 0;
    int y = 0;
    int z = l;
    while(x < numO && y < numT){
        if((*comp)(L[x], R[y]) < 0){
            data[z] = L[x];
            x++;
        }else{
            data[z] = R[y];
            y++;
        }
        z++;
    }
    while(x < numO){
        data[z] = L[x];
        x++;
        z++;
    }
    while(y < numT){
        data[z] = R[y];
        y++;
        z++;
    }
}

void mergeSort(void *data[], int l, int r, int (*comp)(void *, void *)){
    if(l < r){
        int mid = l + (r - l) / 2;
        mergeSort((void *)data, l, mid, (int (*)(void*, void*))(num ? ncmp : strcmp)); 
        mergeSort((void *)data, mid + 1, r, (int (*)(void*, void*))(num ? ncmp : strcmp)); 
        merge((void *)data, l, mid, r, (int (*)(void*, void*))(num ? ncmp : strcmp));
    }
}

void getElem(char *repo, char *res){
    FILE *record;
    char * l = NULL;
    size_t len = 0;
    ssize_t getElem;
    record = fopen(repo, "r");
    int i = 0;
    char ch[200] = {0};
    if (record == NULL) {
        perror("WARNING: Error opening or creating file.");
        return;
    }
    while(fgets(ch, 100, record)){
        data[i] = (char*)malloc(strlen(ch) + sizeof(char*));
        strcpy(data[i], ch);
        i++;
        size++;
    }
    if(fclose(record)){
        printf("WARNING: Error closing.\n");
    }
    mergeSort((void *)data, 0, size-1, (int (*)(void*, void*))(num ? ncmp : strcmp));
    print(res);
}

int main(int argc, char **argv)
{
    if(argc < 2){
        printf("WARNING: Missing Parameters.\n");
        return 0;
    }
    if(argc == 2 && strcmp(argv[1], "-n") == 0){
        printf("WARNING: Incomplete Parameters.\n");
        return 0;
    }
    if(argc == 3 && strcmp(argv[1], "-n") == 0){
        num = 1; 
    }
    if(num == 0){
        getElem(argv[1], "sorted_string.txt");
    }else{
        getElem(argv[2], "sorted_numbers.txt");
    }
    return 0;
}