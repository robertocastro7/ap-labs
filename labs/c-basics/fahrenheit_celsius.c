#include <stdio.h>
#include <stdlib.h>

#define   LOWER  0       /* lower limit of table */
#define   UPPER  100     /* upper limit */
#define   STEP   10      /* step size */

/* print Fahrenheit-Celsius table */

int main(int argc, char *argv[])
{
    int fahr, cel;
    float l, u, s;

    if(argc==2){
        //printf("Enter temperature in Fahrenheit: ");
        //scanf("%f", &fahr);
        //printf("Fahrenheit %.2f = Celsius %.2f", fahr, cel);
        //int cel = (5.0/9.0)*(fahr-32);
        //printf("Fahrenheit %.2f = Celsius %.2f", fahr, cel);
        int fahr = atoi(argv[1]);
        printf("Fahrenheit: %3d, Celsius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
        return 0;
    }else if(argc==4){
        //printf("Enter the lower limit: ");
        //scanf("%f", &l);
        //printf("Enter the upper limit: ");
        //scanf("%f", &u);
        //printf("Enter the step: ");
        //scanf("%f", &s);
        //for (fahr = l; fahr <= u; fahr = fahr + s)
        int l = atoi(argv[1]);
        int u = atoi(argv[2]);
        int s = atoi(argv[3]);
        for (fahr = l; fahr <= u; fahr = fahr + s)
	    printf("Fahrenheit: %3d, Celsius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
        return 0;
    }else{
        printf("ERROR, need 1 or 3 data inputs.");
    }
}