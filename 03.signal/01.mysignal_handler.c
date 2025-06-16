#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static int cnt_int = 0;
static int cnt_quit = 0;
static int cnt_term = 0;
static int cnt_tstp = 0;

void handler_sigint(int signo){
	fprintf(stderr, "Signal handler triggered %d times (signum=%d)\n", cnt_int++, signo);
	if(cnt_int >= 5)
		signal(SIGINT, SIG_DFL);
	if(signo == 15){
	
		sleep(3);
		exit(0);
	}
}

void handler_sigquit(int signo){
	fprintf(stderr, "Signal handler triggered %d times (signum=%d)\n", cnt_quit++, signo);
	if(cnt_quit >= 5)
		signal(SIGQUIT, SIG_DFL);
	if(signo == 15){

		sleep(3);
		exit(0);
	}
}

void handler_sigterm(int signo) {

    cnt_term++;

    fprintf(stderr, "[SIGTERM] count=%d, signo=%d\n", cnt_term, signo);

    if (cnt_term >= 5) {
	    signal(SIGTERM, SIG_DFL);
    }
}

void handler_sigtstp(int signo){
	fprintf(stderr, "Signal handler triggered %d times (signum=%d)\n", cnt_tstp++, signo);
	if(cnt_tstp >= 5)
		signal(SIGTSTP, SIG_DFL);
	if(signo == 15){

		sleep(3);
		exit(0);
	}
}

int main(int argc, char *argv[]){
	signal(SIGINT, handler_sigint);
	signal(SIGQUIT, handler_sigquit);
	signal(SIGTERM, handler_sigterm);
	signal(SIGTSTP, handler_sigtstp);
	while(1){
		printf("signal interrupt test --- INT:%d, QUIT:%d, TERM:%d, TSTP:%d\n", cnt_int, cnt_quit, cnt_term, cnt_tstp);
		sleep(1);
	}
	return 0;
}
