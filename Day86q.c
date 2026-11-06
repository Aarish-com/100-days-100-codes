// Thread pool with a mutex/condvar work queue. Computes sum of squares in parallel chunks.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef void (*task_fn)(void *);
typedef struct Task { task_fn fn; void *arg; struct Task *next; } Task;

typedef struct {
    pthread_mutex_t mu;
    pthread_cond_t  work, idle;
    Task *head, *tail;
    int pending, stop, n;       // pending = queued + running
    pthread_t *th;
} Pool;

static void *worker(void *p) {
    Pool *pl = p;
    for (;;) {
        pthread_mutex_lock(&pl->mu);
        while (!pl->head && !pl->stop) pthread_cond_wait(&pl->work, &pl->mu);
        if (!pl->head) { pthread_mutex_unlock(&pl->mu); return NULL; }
        Task *t = pl->head; pl->head = t->next; if (!pl->head) pl->tail = NULL;
        pthread_mutex_unlock(&pl->mu);

        t->fn(t->arg); free(t);

        pthread_mutex_lock(&pl->mu);
        if (--pl->pending == 0) pthread_cond_broadcast(&pl->idle);
        pthread_mutex_unlock(&pl->mu);
    }
}

static void pool_init(Pool *pl, int n) {
    *pl = (Pool){ .n = n };
    pthread_mutex_init(&pl->mu, NULL); pthread_cond_init(&pl->work, NULL); pthread_cond_init(&pl->idle, NULL);
    pl->th = malloc(n * sizeof(pthread_t));
    for (int i = 0; i < n; i++) pthread_create(&pl->th[i], NULL, worker, pl);
}

static void pool_submit(Pool *pl, task_fn fn, void *arg) {
    Task *t = malloc(sizeof *t); *t = (Task){ fn, arg, NULL };
    pthread_mutex_lock(&pl->mu);
    if (pl->tail) pl->tail->next = t; else pl->head = t;
    pl->tail = t; pl->pending++;
    pthread_cond_signal(&pl->work);
    pthread_mutex_unlock(&pl->mu);
}

static void pool_wait(Pool *pl) {
    pthread_mutex_lock(&pl->mu);
    while (pl->pending) pthread_cond_wait(&pl->idle, &pl->mu);
    pthread_mutex_unlock(&pl->mu);
}

static void pool_destroy(Pool *pl) {
    pthread_mutex_lock(&pl->mu); pl->stop = 1; pthread_cond_broadcast(&pl->work); pthread_mutex_unlock(&pl->mu);
    for (int i = 0; i < pl->n; i++) pthread_join(pl->th[i], NULL);
    free(pl->th);
}

typedef struct { unsigned long long lo, hi, result; } Chunk;

static void sum_squares(void *arg) {
    Chunk *c = arg; unsigned long long s = 0;
    for (unsigned long long i = c->lo; i < c->hi; i++) s += i * i;
    c->result = s;                     // each task writes its own slot: no race
}

int main(void) {
    enum { N = 1000000, CHUNKS = 16 };
    Pool pool; pool_init(&pool, 4);
    Chunk chunks[CHUNKS];
    for (int i = 0; i < CHUNKS; i++) {
        chunks[i] = (Chunk){ (unsigned long long)i * N / CHUNKS + 1, (unsigned long long)(i + 1) * N / CHUNKS + 1, 0 };
        pool_submit(&pool, sum_squares, &chunks[i]);
    }
    pool_wait(&pool);
    unsigned long long total = 0;
    for (int i = 0; i < CHUNKS; i++) total += chunks[i].result;
    unsigned long long n = N, expect = n * (n + 1) * (2 * n + 1) / 6;
    printf("parallel=%llu formula=%llu %s\n", total, expect, total == expect ? "OK" : "MISMATCH");
    pool_destroy(&pool);
    return 0;
}
