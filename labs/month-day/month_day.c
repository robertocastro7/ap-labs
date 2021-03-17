#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* month_day function's prototype*/
void month_day(int y, int yearday, int *pMonth, int *pDay){
    char *yearMonths[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    printf("%s %d, %d\n", yearMonths[*pMonth], *pDay, y);
};

int main(int argc, char **argv) {
    int dMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int yMonths[13] = {0,1,2,3,4,5,6,7,8,9,10,11,12};
    int y = atoi(argv[1]);
    int d = atoi(argv[2]);
    int pDay = 0;
    int pMonth = 0;
    int pC = 0;
    if(argc < 3){
        printf("WARNING: Usage Error\n");
        return -1;
        pC++;
    }
    if (d < 1 || d > 366){
        printf("Not Valid Date\n");
        pC++;
    }else{
        if(((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0)){
            dMonth[1]=29;
        }
        for (int i = 0; i < 12; i++){
            if (d <= dMonth[i]){
                pMonth = i;
                pDay = d;
                break;
            }
            else{
                d -= dMonth[i];
            }
        }
        month_day(y, d, &pMonth, &pDay);
    }
    return 0;
}