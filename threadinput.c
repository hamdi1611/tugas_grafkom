#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <pthread.h>

#define FALSE 0
#define TRUE 1

char isInput = FALSE;
char input;

void *readInput(){
	for(;;){
		if(input = getchar()){
			isInput = TRUE;
		}
	}
}

int main(){
	pthread_t tid;
	system ("/bin/stty raw");
	pthread_create(&tid, NULL, readInput, (void *)&tid);
	for(;;){
		if(isInput){
			isInput = FALSE;
			printf("\n\ryou typed: %c\n\r", input);
			fflush(stdout);
		}
	}
	pthread_exit(NULL); 
	return 0;
}
