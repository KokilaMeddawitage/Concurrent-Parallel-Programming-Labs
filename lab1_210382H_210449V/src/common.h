#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_VALUE 65536

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef enum { OP_MEMBER = 0, OP_INSERT = 1, OP_DELETE = 2 } OpType;

typedef struct {
    OpType op;
    int key;
} Task;

typedef struct {
    size_t start;
    size_t end;
    Task* ops;
} ThreadSlice;

extern Node* head;
extern pthread_mutex_t list_mutex;
extern pthread_rwlock_t list_rwlock;

int Member_serial(int value, Node* head);
int Insert_serial(int value, Node** head);
int Delete_serial(int value, Node** head);

double now_ms();
void shuffle_tasks(Task* a, size_t n, unsigned int seed);
void populate_unique_initial(Node** head, int n, unsigned int seed);
void build_ops(Task* ops, size_t m, double p_member, double p_insert, double p_delete, unsigned int seed);

#endif
