#include <unistd.h>
#include <sys/_types/_ssize_t.h>
#include "apue.h"
#include <fcntl.h>

#define BUFSIZE 4096

void set_fl(int fd, int flags)
{
   int val;
   if ((val = fcntl(fd, F_GETFL, 0)) < 0)
   {
      err_sys("fcntl error for fd %d", fd);
   }

   val |= flags;

   if (fcntl(fd, F_SETFL, val) < 0)
   {
      err_sys("fcntl F_SETFL error");
   }
}

int main(int argc, char *argv[])
{
   ssize_t n;
   int inputfd, outputfd;
   char buf[BUFSIZE];
   int loop = 0;

   if (argc != 3)
   {
      err_quit("usage: ./a.out <input> <output>");
   }

   if ((inputfd = open(argv[1], O_RDONLY)) < 0)
   {
      err_sys("can't open %s", argv[1]);
   }

   if ((outputfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC | 0644)) < 0)
   {
      err_sys("can't create %s", argv[2]);
   }

   while ((n = read(inputfd, buf, BUFSIZE)) > 0)
   {
      if (write(outputfd, buf, n) != n)
      {
         err_sys("write error");
      }
      loop++;
   }
   fdatasync(outputfd);

   if (n < 0)
   {
      err_sys("read error");
   }

   dprintf(STDOUT_FILENO, "loops: %d\n", loop);

   return 0;
}

/**
 * Operation            Clock time     User time      System Time
 * write to disk file   1.63s          37.01 millis   800.03 millis
 * write with O_SYNC    5.11s          0.04s          2.06s
 * write with fsync     5.03s          0.04s          2.00s
 * write with fdatasync 4.48s          0.04s          2.02s
 */