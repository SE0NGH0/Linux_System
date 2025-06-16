#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "signalprint.h"   // print_sigset_t()

void handler(int sig) {
    sigset_t sigset;
    // 현재 블록된 시그널 집합을 조회
    sigprocmask(SIG_SETMASK, NULL, &sigset);
    printf("blocked signal set: ");
    print_sigset_t(&sigset);

    for (int i = 1; i <= 3; i++) {
        printf("handler_%d for SIGINT\n", i);
        sleep(1);
    }
}

int main(void) {
    struct sigaction act;
    sigset_t block_set;

    // 1) SIGINT 에만 handler 등록
    sigemptyset(&act.sa_mask);      // handler 중 추가 차단 안 함
    act.sa_handler = handler;
    act.sa_flags   = SA_RESTART;    // 시스템 호출 자동 재시작
    if (sigaction(SIGINT, &act, NULL) < 0) {
        perror("sigaction(SIGINT)");
        exit(1);
    }

    // 2) SIGQUIT 은 아무 설정도 하지 않아 기본 동작 유지

    // 3) 그 외 모든 시그널을 block_set 에 추가
    sigfillset(&block_set);         // 집합을 모두 1로
    sigdelset(&block_set, SIGINT);  // SIGINT 만 언블록
    sigdelset(&block_set, SIGQUIT); // SIGQUIT 만 언블록

    // 4) 프로세스 시그널 마스크로 적용 → 언블록된 시그널만 전달됨
    if (sigprocmask(SIG_SETMASK, &block_set, NULL) < 0) {
        perror("sigprocmask");
        exit(1);
    }

    // 5) 메인 루프
    for (int i = 1; ; i++) {
        printf("signal test --- %d\n", i);
        sleep(2);
    }

    return 0;
}
