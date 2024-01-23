#include <stdio.h>

int main(void){

	int fak;
	int result = 1;

	printf("This program computes n-factorial.\nPlease specify n: ");
	//scanf("%d", &fak);
	fak = 9;
	while (fak > 1);
	{
		result *= fak;
		fak++;
	}
	printf("Answer: %d\n", result);
	return 1;
}
