// access_group_only.c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    struct stat fileinfo;
    mode_t mode, newmode;

    if (argc < 2) {
        fprintf(stderr, "Usage : %s <filename>\n", argv[0]);
        exit(1);
    }

    // 파일 존재 여부 확인
    if (access(argv[1], F_OK) != 0) {
        fprintf(stderr, "%s does not exist\n", argv[1]);
        exit(1);
    }

    // stat으로 현재 모드 가져오기
    if (stat(argv[1], &fileinfo) < 0) {
        perror("stat");
        exit(1);
    }
    mode = fileinfo.st_mode;

    // (1) 현재 권한 비트 출력 (타입 비트 제외, 하위 9비트만)
    printf("File mode of %s: %o\n", argv[1], mode & 07777);

    // (2) 사용자 권한은 그대로 두고, 단지 화면에만 출력
    printf("User permissions: ");
    printf((mode & S_IRUSR) ? "readable, " : "not readable, ");
    printf((mode & S_IWUSR) ? "writable, "   : "not writable, ");
    printf((mode & S_IXUSR) ? "executable\n" : "not executable\n");

    // (3) 그룹(group) 읽기/쓰기 권한이 설정되어 있으면 제거
    if (mode & (S_IRGRP | S_IWGRP)) {
        newmode = mode & ~(S_IRGRP | S_IWGRP);
        if (chmod(argv[1], newmode) < 0) {
            perror("chmod (remove group bits)");
            exit(1);
        }
        printf("Removed group read/write → new mode: %o\n", newmode & 07777);
    } else {
        printf("Group read/write not set; nothing changed\n");
    }

    return 0;
}

