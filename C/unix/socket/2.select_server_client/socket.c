struct SocketArray {
    int *sockets; // Dynamic array of socket descriptors
    size_t size; // Current number of sockets
    size_t capacity; // Total capacity
};

struct SocketArray *create_socket_array(const size_t initial_capacity) {
    struct SocketArray *sa = malloc(sizeof(struct SocketArray));
    if (!sa) return NULL;

    sa->sockets = malloc(initial_capacity * sizeof(int));
    if (!sa->sockets) {
        free(sa);
        return NULL;
    }

    sa->size = 0;
    sa->capacity = initial_capacity;
    return sa;
}

void add_socket(struct SocketArray *sa, int sock_fd) {
    if (sa->size >= sa->capacity) {
        size_t new_capacity = sa->capacity * 2;
        int *new_sockets = realloc(sa->sockets, new_capacity * sizeof(int));
        if (!new_sockets) return;

        sa->sockets = new_sockets;
        sa->capacity = new_capacity;
    }

    sa->sockets[sa->size++] = sock_fd;
}

void remove_socket(struct SocketArray *sa, int sock_fd) {
    for (size_t i = 0; i < sa->size; i++) {
        if (sa->sockets[i] == sock_fd) {
            memmove(&sa->sockets[i], &sa->sockets[i + 1],
                    (sa->size - i - 1) * sizeof(int));
            sa->size--;
            return;
        }
    }
}

void free_socket_array(struct SocketArray *sa) {
    free(sa->sockets);
    free(sa);
}
