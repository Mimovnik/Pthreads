#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAX_THREADS 20
#define ARRAY_SIZE 1e9

typedef struct {
  int *array;
  int start;
  int end;
} params;

void *f(void *arg) {
  // printf("Thread %lu active\n", pthread_self());

  params *p = (params *)arg;

  for (int i = p->start; i < p->end; i++) {
    p->array[i] += 1;
  }

  // printf("Thread %lu closing\n", pthread_self());
  free(p);
  return NULL;
}

int run(int thread_number) {
  int *array = (int *)calloc(ARRAY_SIZE, sizeof(int));
  if (array == NULL) {
    perror("array calloc failed");
    return 1;
  }

  pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_number);
  if (threads == NULL) {
    perror("threads malloc failed");
    return 1;
  }
  for (int i = 0; i < thread_number; i++) {
    params *p = (params *)malloc(sizeof(params));
    if (p == NULL) {
      perror("p malloc failed");
      return 1;
    }
    p->array = array;
    p->start = i * ARRAY_SIZE / thread_number;
    if (i == thread_number - 1) {
      p->end = ARRAY_SIZE;
    } else {
      p->end = (i + 1) * ARRAY_SIZE / thread_number;
    }
    pthread_t thread;
    int t = pthread_create(&thread, NULL, f, p);
    if (t != 0) {
      printf("pthread_create failed with code %d", t);
      return t;
    }
    threads[i] = thread;
  }

  for (int i = 0; i < thread_number; i++) {
    int t = pthread_join(threads[i], NULL);
    if (t != 0) {
      printf("pthread_join failed with code %d", t);
      return t;
    }
  }

  free(threads);
  free(array);
  return 0;
}

int main() {
  for (int i = 1; i <= MAX_THREADS; i++) {
    printf("Running program on %d threads:\n", i);
    struct timeval start, end, total;
    gettimeofday(&start, NULL);

    int t = run(i);
    if (t != 0) {
      printf("run %d failed", i);
      return t;
    }

    gettimeofday(&end, NULL);
    timersub(&end, &start, &total);
    printf("Total execution time on %d threads: %ld.%06ld seconds\n", i,
           total.tv_sec, total.tv_usec);
  }
}
