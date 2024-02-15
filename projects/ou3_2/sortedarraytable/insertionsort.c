#include <stdlib.h>
#include <stdio.h>

int main(void)
{
	int array[] = {1, 3, 5, 4, 8, 9, 7, 0 };
	int array_length = sizeof(array) / sizeof(array[0]);

	for (int i = 1; i < array_length; i++)
	{
		int temp = array[i];
		int j = i - 1;
		
		while (j >= 0 && array[j] > temp)
		{
			array[j + 1] = array[j];
			j--;
		}
		array[j + 1] = temp;
	}
	
	fprintf(stderr, "{ ");
	for (int i = 0; i < array_length; i++)
	{
		fprintf(stderr, "%d ", array[i]);
	}
	fprintf(stderr, " }");
}
