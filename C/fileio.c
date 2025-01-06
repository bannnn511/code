//
// Created by ban on 6/1/25.
//

#include <sys/fcntl.h>
#include "util.h"
#include <unistd.h>



int main(void) {
    int fd = open("./test", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | O_CLOEXEC);
    if (fd == -1) {
        handle_error(errno, "open");
    }

    int n;
    char buf[BUFSIZ];
    while ((n = read(STDIN_FILENO, buf, BUFSIZ)) != 0) {
        if (write(fd, buf, n) != n) {
            handle_error(errno, "write error");
        }
    }


    return 0;
}

