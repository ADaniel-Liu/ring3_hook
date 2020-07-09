#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, const char *argv[]) {
    FILE *fd = fopen("a.txt", "r");
    if (fd) {
        printf("file opened succesfully\n");
        fclose(fd);
    } else {
        printf("%s: cannot open the file\n", strerror(errno));
    }
    return 0;
}

