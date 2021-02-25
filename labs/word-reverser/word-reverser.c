#include <stdio.h>

#define IN   1
#define OUT  0

void reverse(char array[], int length)
{
	char tmp;
    for (int j = 0;  j < length/2; j++) {
		tmp = array[j];
		array[j] = array[length - j - 1];
		array[length - j - 1] = tmp;
    }
	printf("%s", array);
}

int main()
{
    int i, state;
	char c, word[100];
    state = IN;
    i = 0;
    while ((c = getchar()) != EOF) {
		if (c == ' ' || c == '\n' || c == '\t'){
			state = OUT;
			printf("%s", word);
			printf("\n");
			//REVERSE
			reverse(word, i);
			i = 0;
		}
		else if (state == OUT) {
			state = IN;
			i = 0;
		}else{
			word[i] = c;
			i++;
		}
    }
    return 0;
}