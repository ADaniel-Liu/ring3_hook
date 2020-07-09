#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, const char *argv[]) {
    int fd = open("a.txt", O_RDONLY | O_CREAT, 0644);
    if (fd != -1) {
        printf("file opened succesfully\n");
        close(fd);
    } else {
        printf("%s: cannot open the file\n", strerror(errno));
    }
    return 0;
}

