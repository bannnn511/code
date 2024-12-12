#include <pthread.h>
#include <stdio.h>
#include <time.h>

// START: wait and signal
int done = 0;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

void thr_exit() {
  pthread_mutex_lock(&m);
  done = 1;
  pthread_cond_signal(&c);
  pthread_mutex_unlock(&m);
}

void *child(void *arg) {
  printf("child\n");
  thr_exit();
  return NULL;
}

void thr_join() {
  pthread_mutex_lock(&m);
  while (done == 0) {
    pthread_cond_wait(&c, &m);
  }
  pthread_mutex_unlock(&m);
}

// END: wait and signal

// START: bounded buffer

#define MAX 100;

int buffer[100];
int fill_ptr = 0;
int use_ptr = 0;
int count = 0;

void put(int value) {
  buffer[fill_ptr] = value;
  fill_ptr = (fill_ptr + 1) % MAX;
  count++;
}

int get() {
  int tmp = buffer[use_ptr];
  use_ptr = (use_ptr + 1) % MAX;
  count--;

  return tmp;
}

int loops;
pthread_cond_t empty, fill;
pthread_mutex_t mutex;

void *producer(void *arg) {
  for (int i = 0; i < loops; i++) {
    pthread_mutex_lock(&mutex);
    while (count == 1) {
      pthread_cond_wait(&empty, &mutex);
    }
    put(i);
    pthread_cond_signal(&fill);
    pthread_mutex_unlock(&mutex);
  }

  return NULL;
}

void *consumer(void *arg) {
  for (int i = 0; i < loops; i++) {
    pthread_mutex_lock(&mutex);
    while (count == 0) {
      pthread_cond_wait(&fill, &mutex);
    }
    const int tmp = get();
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&m);
    printf("%d\n", tmp);
  }

  return NULL;
}

// END: bounded buffer

int main(int argc, char *argv[]) {
  printf("parent: begin\n");
  pthread_t p;
  pthread_create(&p, NULL, child, NULL);
  thr_join();
  printf("parent: end\n");
  return 0;
}
