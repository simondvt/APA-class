#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

typedef struct queue *Queue;

Queue QInit(void);
void enqueue(Queue q, int val);
int dequeue(Queue q);
int isEmpty(Queue q);

#endif // QUEUE_H_INCLUDED
