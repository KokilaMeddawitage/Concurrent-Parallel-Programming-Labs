#define _GNU_SOURCE
#include "common.h"

Node* head = NULL;
pthread_mutex_t list_mutex;       // unused in serial
pthread_rwlock_t list_rwlock;     // unused in serial

static void* worker_serial(void* arg) {
    ThreadSlice* s = (ThreadSlice*)arg;
    for (size_t i = s->start; i < s->end; ++i) {
        Task t = s->ops[i];
        switch (t.op) {
            case OP_MEMBER: (void)Member_serial(t.key, head); break;
            case OP_INSERT: (void)Insert_serial(t.key, &head); break;
            case OP_DELETE: (void)Delete_serial(t.key, &head); break;
        }
    }
    return NULL;
}

int main(int argc, char** argv) {
    if (argc != 6) {
        fprintf(stderr, "Usage: %s n m p_member p_insert p_delete\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    size_t m = (size_t)atol(argv[2]);
    double p_member = atof(argv[3]);
    double p_insert = atof(argv[4]);
    double p_delete = atof(argv[5]);
    if (p_member + p_insert + p_delete < 0.999 || p_member + p_insert + p_delete > 1.001) {
        fprintf(stderr, "Fractions must sum to 1.0\n");
        return 1;
    }

    unsigned int seed = (unsigned int)(time(NULL) ^ (uintptr_t)&argc);
    populate_unique_initial(&head, n, seed);

    Task* ops = (Task*)malloc(sizeof(Task) * m);
    if (!ops) { perror("malloc"); return 1; }
    build_ops(ops, m, p_member, p_insert, p_delete, seed ^ 0x12345u);

    ThreadSlice slice = { .start = 0, .end = m, .ops = ops };

    // TIME ONLY THE m-ops SECTION
    double t0 = now_ms();
    worker_serial(&slice);
    double t1 = now_ms();

    printf("elapsed_ms,%.3f\n", (t1 - t0));

    free(ops);
    // NOTE: we don't free the entire list to keep code short
    return 0;
}
