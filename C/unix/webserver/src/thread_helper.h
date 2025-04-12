#include <assert.h>

#define thread_create_or_die(tid, attr, start_routine, arg)     \
    ({                                                          \
        int rc = pthread_create(tid, attr, start_routine, arg); \
        assert(rc == 0);                                        \
        rc;                                                     \
    })

#define thread_mutex_init(mutex, attr) \
    ({                                                          \
        int rc = pthread_mutex_init(mutex, attr);              \
        assert(rc == 0);                                        \
        rc;                                                     \
    })

#define thread_cond_init(cond, attr) \
    ({                                                          \
        int rc = pthread_cond_init(cond, attr);                \
        assert(rc == 0);                                        \
        rc;                                                     \
    })

#define thread_mutex_lock(mutex) \
    ({                                                        \
        int rc = pthread_mutex_lock(mutex);                  \
        assert(rc == 0);                                      \
        rc;                                                   \
    })

#define thread_mutex_unlock(mutex) \
    ({                                                          \
        int rc = pthread_mutex_unlock(mutex);                  \
        assert(rc == 0);                                        \
        rc;                                                     \
    })

#define thread_cond_signal(cond) \
    ({                                                          \
        int rc = pthread_cond_signal(cond);                     \
        assert(rc == 0);                                        \
        rc;                                                     \
    })

#define thread_cond_wait(cond, mutex) \
    ({                                                          \
        int rc = pthread_cond_wait(cond, mutex);               \
        assert(rc == 0);                                        \
        rc;                                                     \
    })

#define thread_cond_broadcast(cond) \
    ({ \
int rc = pthread_cond_broadcast(cond); \
assert(rc == 0); \
rc; \
})

