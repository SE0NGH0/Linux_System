#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Read PPid from /proc/[pid]/status
long get_ppid(long pid) {
    char path[64], line[128];
    FILE *f;
    snprintf(path, sizeof(path), "/proc/%ld/status", pid);
    if ((f = fopen(path, "r")) == NULL) return -1;
    while (fgets(line, sizeof(line), f)) {
        long p;
        if (sscanf(line, "PPid:\t%ld", &p) == 1) {
            fclose(f);
            return p;
        }
    }
    fclose(f);
    return -1;
}

int main(void) {
    long pid = getpid(), ppid;
    printf("01.current : %ld", pid);

    while ((ppid = get_ppid(pid)) > 2) {
        printf(" → %ld", ppid);
        pid = ppid;
    }
    if (ppid > 0)
        printf(" → %ld", ppid);

    printf("\n");
    return 0;
}
