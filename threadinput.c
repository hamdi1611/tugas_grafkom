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

enum state {RECEIVED, WAITING, STOP};
enum state isInput = WAITING;
char input;
char stop_input = 'a';

void *readInput(){
	while(isInput != STOP){
		while(isInput == RECEIVED);
		if(input = getchar()){
			if(input == 27){
				getchar();
				input = getchar();
			}
			if(input == stop_input){
				isInput = STOP;
			}
			else{
				isInput = RECEIVED;
			}
		}
	}
}

int main(){
	pthread_t tid;
	system ("/bin/stty raw");
	pthread_create(&tid, NULL, readInput, (void *)&tid);
	for(;;){
		//do something here while the other thread read input
		if(isInput == RECEIVED){
			printf("\n\ryou typed: %c\n\r", input);
			fflush(stdout);
			isInput = WAITING;
		}
		else if(isInput == STOP){
			break;
		}
	}
	pthread_exit(NULL); 
	return 0;
}
