#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/* This program is supposed to test if you get the same random numbers
   if you reset the random number generator with a certain seed
   value. */
int main(void){

	int i, j, seed, failed, tested; 
	int **mybuffer;
    
	printf("Please enter a seed for the random number generator (any integer): "); 
	scanf("%d",&seed); 
	srand(seed); 

	/* Allocate a bunch of pointers */
	mybuffer = calloc(100,sizeof(int *)); 
	if (mybuffer == NULL){
		fprintf(stderr, "Failed to allocate memory!"); 
		exit (-1); 
	}
	/* Allocate memory to each pointer */
	for (i=0 ; i<100 ; i++){
		mybuffer[i] = calloc(10,sizeof(int));	 
		if (mybuffer[i] == NULL){
			fprintf(stderr, "Failed to allocate memory!"); 
			exit (-1); 
		}   
	}
	/* Fill the allocated memory with random numbers */
	for (i=0 ; i<100 ; i++){
		for (j=0 ; j<10 ; j++){
			mybuffer[i][j] = rand();    
		}
	}

	/* Lets try to see if we get the same random numbers again. */
	/* Reset the seed to the random number generator. */
	srand(seed);	     
	failed = 0; 
	tested = 0; 
	for (i=0 ; i<100 ; i++)
		for(j=0 ; j<10 ; j++)
			if (mybuffer[i][j] != rand())
				printf("Random number mismatch!\n"); 
				tested++; 
				failed = 1; 
	    
	if (failed)
		printf("Something went wrong...\n"); 
	else
		printf("%d number verified and correct\n",tested);  

	/* Release the memory */
	for (i=0 ; i<100; i++){
		free(mybuffer[i]); 
	}
	free(mybuffer); 
	return 1; 
}
