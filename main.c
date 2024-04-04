#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define THREADS 10
#define ARRAY_SIZE 1e9

typedef struct {
  int *array;
  int start;
  int end;
} params;

void *f(void *arg) {
  printf("Thread %lu active\n", pthread_self());

  params *p = (params *)arg;

  for (int i = p->start; i < p->end; i++) {
    p->array[i] += 1;
  }

  printf("Thread %lu closing\n", pthread_self());
  free(p);
  return NULL;
}

int main() {
  struct timeval start, end, total;
  gettimeofday(&start, NULL);

  int *array = (int *)calloc(ARRAY_SIZE, sizeof(int));

  pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * THREADS);
  for (int i = 0; i < THREADS; i++) {
    params *p = (params *)malloc(sizeof(params));
    p->array = array;
    p->start = i * ARRAY_SIZE / THREADS;
    if (i == THREADS - 1) {
      p->end = ARRAY_SIZE;
    } else {
      p->end = (i + 1) * ARRAY_SIZE / THREADS;
    }
    pthread_t thread;
    threads[i] = thread;
    pthread_create(&thread, NULL, f, p);
  }

  for (int i = 0; i < THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  gettimeofday(&end, NULL);
  timersub(&end, &start, &total);
  printf("Total execution time: %ld.%06ld seconds\n", total.tv_sec, total.tv_usec);

  free(threads);
  free(array);
  return 0;
}
