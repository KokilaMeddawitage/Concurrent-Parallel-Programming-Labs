#define _GNU_SOURCE
#include "common.h"

Node* head = NULL;
pthread_mutex_t list_mutex; // unused
pthread_rwlock_t list_rwlock = PTHREAD_RWLOCK_INITIALIZER;

typedef struct {
    ThreadSlice base;
} ThreadArgs;

static int Member_rw(int value) {
    pthread_rwlock_rdlock(&list_rwlock);
    int r = Member_serial(value, head);
    pthread_rwlock_unlock(&list_rwlock);
    return r;
}
static int Insert_rw(int value) {
    pthread_rwlock_wrlock(&list_rwlock);
    int r = Insert_serial(value, &head);
    pthread_rwlock_unlock(&list_rwlock);
    return r;
}
static int Delete_rw(int value) {
    pthread_rwlock_wrlock(&list_rwlock);
    int r = Delete_serial(value, &head);
    pthread_rwlock_unlock(&list_rwlock);
    return r;
}

static void* worker(void* arg) {
    ThreadArgs* a = (ThreadArgs*)arg;
    ThreadSlice* s = &a->base;
    for (size_t i = s->start; i < s->end; ++i) {
        Task t = s->ops[i];
        switch (t.op) {
            case OP_MEMBER: (void)Member_rw(t.key); break;
            case OP_INSERT: (void)Insert_rw(t.key); break;
            case OP_DELETE: (void)Delete_rw(t.key); break;
        }
    }
    return NULL;
}

int main(int argc, char** argv) {
    if (argc != 7) {
        fprintf(stderr, "Usage: %s n m p_member p_insert p_delete threads\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    size_t m = (size_t)atol(argv[2]);
    double p_member = atof(argv[3]);
    double p_insert = atof(argv[4]);
    double p_delete = atof(argv[5]);
    int threads = atoi(argv[6]);
    if (threads <= 0) { fprintf(stderr, "threads must be > 0\n"); return 1; }
    if (p_member + p_insert + p_delete < 0.999 || p_member + p_insert + p_delete > 1.001) {
        fprintf(stderr, "Fractions must sum to 1.0\n");
        return 1;
    }

    unsigned int seed = (unsigned int)(time(NULL) ^ (uintptr_t)&argc);
    // Initial population under write lock for safety
    for (int i = 0; i < n; ++i) {
        int k = rand() % MAX_VALUE;
        pthread_rwlock_wrlock(&list_rwlock);
        while (Member_serial(k, head)) { k = rand() % MAX_VALUE; }
        Insert_serial(k, &head);
        pthread_rwlock_unlock(&list_rwlock);
    }

    Task* ops = (Task*)malloc(sizeof(Task) * m);
    if (!ops) { perror("malloc"); return 1; }
    build_ops(ops, m, p_member, p_insert, p_delete, seed ^ 0x2468u);

    pthread_t* th = (pthread_t*)malloc(sizeof(pthread_t) * threads);
    ThreadArgs* args = (ThreadArgs*)malloc(sizeof(ThreadArgs) * threads);
    size_t chunk = m / threads;
    size_t rem = m % threads;

    // TIME ONLY THE m-ops SECTION
    double t0 = now_ms();
    size_t off = 0;
    for (int i = 0; i < threads; ++i) {
        size_t len = chunk + (i < (int)rem ? 1 : 0);
        args[i].base.start = off;
        args[i].base.end = off + len;
        args[i].base.ops = ops;
        off += len;
        pthread_create(&th[i], NULL, worker, &args[i]);
    }
    for (int i = 0; i < threads; ++i) pthread_join(th[i], NULL);
    double t1 = now_ms();

    printf("elapsed_ms,%.3f\n", (t1 - t0));

    free(ops); free(th); free(args);
    return 0;
}
