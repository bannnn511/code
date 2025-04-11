
#define thread_create_or_die(tid, attr, start_routine, arg)     \
    ({                                                          \
        int rc = pthread_create(tid, attr, start_routine, arg); \
        assert(rc == 0);                                        \
        rc;                                                     \
    })