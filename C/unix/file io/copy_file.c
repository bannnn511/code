#include <unistd.h>
#include <sys/_types/_ssize_t.h>
#include "apue.h"

#define BUFSIZE 4096

int main()
{
   ssize_t n;
   char buf[BUFSIZE];
   int loop = 0;

   while ((n = read(STDIN_FILENO, buf, BUFSIZE)) > 0)
   {
      if (write(STDOUT_FILENO, buf, n) != n)
      {
         err_sys("write error");
      }
      loop++;
   }

   if (n < 0)
   {
      err_sys("read error");
   }

   dprintf(STDERR_FILENO, "loops: %d\n", loop);

   return 0;
}

/**
 ** BUFFSIZE    Clock time      User time       System Time     Loops
 *  2           269.59s         69.78s          197.82s         268435456
 *  4           136.20s         34.91s          99.59s          134217728
 *  8           69.54s          17.45s          50.33s          67108864
 *  16          35.87s          8.80s           25.56s          33554432
 *  32          18.45s          4.35s           12.70s          16777216
 *  64          9.90s           2.19s           6.49s           8388608
 *  128         5.60s           1.10s           3.25s           4194304
 *  256         3.59s           0.55s           1.67s           2097152
 *  512         2.80s           280.25ms        905.16ms        1048576
 *  1024        1.96s           141.18ms        478.08ms        524288
 *  2048        1.98s           72.98ms         287.00ms        262144
 *  4096        1.37s           39.09ms         225.18ms        131072
 *  8192        1.50s           21.54ms         152.83ms        65536
 *  16384       2.04s           11.53ms         130.04ms        32768
 *  32768       1.98s           5.90ms          124.91ms        16384
 */
