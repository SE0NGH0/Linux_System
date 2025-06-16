#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int count = 0;

void handler(int signo){
	printf("Received Signal no - %d\n", signo);
    if(signo == 34){
        count++;
        fprintf (stderr, "signo34, count up, count : %d\n", count);
    }else if(signo == 35){
        count--;
        fprintf (stderr, "signo35, count down, count : %d\n", count);
    }
}

void handler34(int signo){
	count++;
        fprintf (stderr, "signo34, count up, count : %d\n", count);
}

void handler35(int signo){
	count--;
        fprintf (stderr, "signo35, count down, count : %d\n", count);
}

int main(int argc, char *argv[]){
	int a,b;

    printf("pid : %d\n", getpid());

    signal(34, handler34);
    signal(35, handler35);
	

    while(1){
        sleep(1);
    }

	return 0;
}