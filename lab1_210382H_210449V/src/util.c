#include "common.h"

double now_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1e6;
}

int Member_serial(int value, Node* head) {
    Node* curr = head;
    while (curr != NULL && curr->data < value) curr = curr->next;
    return (curr != NULL && curr->data == value);
}

int Insert_serial(int value, Node** head) {
    Node* curr = *head;
    Node* pred = NULL;
    while (curr != NULL && curr->data < value) { pred = curr; curr = curr->next; }
    if (curr != NULL && curr->data == value) return 0; // exists
    Node* n = (Node*)malloc(sizeof(Node));
    if (!n) { perror("malloc"); exit(1); }
    n->data = value;
    n->next = curr;
    if (pred == NULL) *head = n; else pred->next = n;
    return 1;
}

int Delete_serial(int value, Node** head) {
    Node* curr = *head;
    Node* pred = NULL;
    while (curr != NULL && curr->data < value) { pred = curr; curr = curr->next; }
    if (curr == NULL || curr->data != value) return 0;
    if (pred == NULL) *head = curr->next; else pred->next = curr->next;
    free(curr);
    return 1;
}

void shuffle_tasks(Task* a, size_t n, unsigned int seed) {
    // Fisher–Yates
    srand(seed);
    for (size_t i = n - 1; i > 0; --i) {
        size_t j = (size_t)(rand() % (int)(i + 1));
        Task tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    }
}

void populate_unique_initial(Node** head, int n, unsigned int seed) {
    // Use a simple bitmap of 65536 entries to ensure uniqueness.
    uint8_t* used = (uint8_t*)calloc(MAX_VALUE, sizeof(uint8_t));
    if (!used) { perror("calloc"); exit(1); }
    srand(seed);
    int inserted = 0;
    while (inserted < n) {
        int k = rand() % MAX_VALUE;
        if (!used[k]) {
            used[k] = 1;
            Insert_serial(k, head);
            inserted++;
        }
    }
    free(used);
}

void build_ops(Task* ops, size_t m, double p_member, double p_insert, double p_delete, unsigned int seed) {
    size_t cm = (size_t)(m * p_member);
    size_t ci = (size_t)(m * p_insert);
    size_t cd = (size_t)(m * p_delete);
    size_t assigned = cm + ci + cd;
    while (assigned < m) { // top-up remainder to hit exactly m
        if (cm <= ci && cm <= cd) { cm++; }
        else if (ci <= cm && ci <= cd) { ci++; }
        else { cd++; }
        assigned = cm + ci + cd;
    }
    srand(seed ^ 0x9e3779b1u);
    size_t idx = 0;
    for (size_t i = 0; i < cm; ++i) {
        ops[idx].op = OP_MEMBER;
        ops[idx].key = rand() % MAX_VALUE;
        idx++;
    }
    for (size_t i = 0; i < ci; ++i) {
        ops[idx].op = OP_INSERT;
        ops[idx].key = rand() % MAX_VALUE;
        idx++;
    }
    for (size_t i = 0; i < cd; ++i) {
        ops[idx].op = OP_DELETE;
        ops[idx].key = rand() % MAX_VALUE;
        idx++;
    }
    shuffle_tasks(ops, m, seed ^ 0x85ebca6bu);
}
