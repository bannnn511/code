#include <sys/time.h>
#include <assert.h>


double get_seconds() {
    struct timeval t;
    int rc = gettimeofday(&t, NULL);
    assert(rc == 0);
    return (double) ((double) t.tv_sec + (double) t.tv_usec / 1e6);
}