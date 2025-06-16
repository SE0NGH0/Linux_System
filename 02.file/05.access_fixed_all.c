// access_fixed_all.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

static void process_file(const char *fname) {
    struct stat fileinfo;
    mode_t mode, newmode;

    // (0) 파일 존재 여부 확인
    if (access(fname, F_OK) != 0) {
        fprintf(stderr, "%s does not exist\n", fname);
        return;
    }

    // (1) stat으로 현재 모드 가져오기
    if (stat(fname, &fileinfo) < 0) {
        perror("stat");
        return;
    }
    mode = fileinfo.st_mode;

    // (2) 현재 권한 비트 출력 (하위 9비트만)
    printf("File mode of %s: %o\n", fname, mode & 07777);

    // (3) 사용자 권한 출력
    printf("User permissions for %s: ", fname);
    printf((mode & S_IRUSR) ? "readable, " : "not readable, ");
    printf((mode & S_IWUSR) ? "writable, "   : "not writable, ");
    printf((mode & S_IXUSR) ? "executable\n" : "not executable\n");

    // (4) 그룹 읽기/쓰기 비트가 설정되어 있으면 제거
    if (mode & (S_IRGRP | S_IWGRP)) {
        newmode = mode & ~(S_IRGRP | S_IWGRP);
        if (chmod(fname, newmode) < 0) {
            perror("chmod (remove group bits)");
            return;
        }
        printf("Removed group read/write → new mode: %o\n\n", newmode & 07777);
    } else {
        printf("Group read/write not set for %s; nothing changed\n\n", fname);
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        // 인자가 없으면 현재 디렉터리에서 이름에 "file" 포함된 모든 파일 처리
        DIR *dp = opendir(".");
        if (!dp) {
            perror("opendir");
            exit(1);
        }
        struct dirent *entry;
        while ((entry = readdir(dp)) != NULL) {
            if (strstr(entry->d_name, "file")) {
                process_file(entry->d_name);
            }
        }
        closedir(dp);

    } else if (argc == 2) {
        // 인자가 하나면 해당 파일만 처리
        process_file(argv[1]);

    } else {
        fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
        exit(1);
    }

    return 0;
}

