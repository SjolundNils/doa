#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

int fun (int n){
	if (n > 100){
		return n-5;
	}
	return fun(fun(n+11));
}

int main()
{
	int n = 45;
	int ans = fun(45);
	printf("\n%d\n", ans);
	return 0;
}