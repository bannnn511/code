#include <unistd.h>
#include <sys/_types/_ssize_t.h>
#include "apue.h"
#include <fcntl.h>

#define BUFSIZE 4096

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

   // Open input file for reading
   if ((inputfd = open(argv[1], O_RDONLY)) < 0)
   {
      err_sys("can't open %s", argv[1]);
   }

   // Open output file for writing with O_SYNC flag
   if ((outputfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
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

   if (n < 0)
   {
      err_sys("read error");
   }

   dprintf(STDOUT_FILENO, "loops: %d\n", loop);

   return 0;
}
