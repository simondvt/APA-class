#include <stdlib.h>
#include "queue.h"

typedef struct node
{
    int val;
    struct node *next;
} *Node;

struct queue
{
    int n;
    Node head, tail;
};

Queue QInit(void)
{
    Queue q = malloc(sizeof(*q));
    q->head = NULL;
    q->tail = NULL;
    q->n = 0;

    return q;
}

void enqueue(Queue q, int val)
{
    Node n = malloc(sizeof(*n));
    n->val = val;
    n->next = NULL;

    if (q->tail == NULL)
    {
        q->head = q->tail = n;
        q->n++;
        return;
    }

    q->tail->next = n;
    q->tail = n;

    q->n++;
}

int dequeue(Queue q)
{
    Node cur;
    int val;

    cur = q->head;
    val = cur->val;
    q->head = cur->next;

    if (q->head == NULL)
        q->tail = NULL;

    free(cur);
    q->n--;

    return val;
}

int isEmpty(Queue q)
{
    return q->n == 0;
}