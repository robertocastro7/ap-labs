#include <stdio.h>

#define   LOWER  0       /* lower limit of table */
#define   UPPER  100     /* upper limit */
#define   STEP   10      /* step size */

/* print Fahrenheit-Celsius table */

int main()
{
    int fahr, cel;
    float l, u, s;

    printf("Enter temperature in Fahrenheit: ");
    scanf("%f", &fahr);
    cel = (5.0/9.0)*(fahr-32);
    printf("%.2f Celsius = %.2f Fahrenheit", cel, fahr);
    return 0;
    
    printf("Enter the lower limit: ");
    scanf("%f", &l);
    printf("Enter the upper limit: ");
    scanf("%f", &u);
    printf("Enter the step: ");
    scanf("%f", &s);
    for (fahr = l; fahr <= u; fahr = fahr + s)
	printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));

    return 0;
}