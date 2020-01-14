
#include "SqQueue.h"

#include "x_malloc.h"


/*��ʼ������*/
uint8 InitQueue(SqQueue *q,uint16 size){
	uint8* data;
	if (q == NULL) { return FALSE; }
	data = xMalloc(sizeof(QueueDateType)*size);
	if (data == NULL) { return FALSE; }
	q->maxVal = size;
	q->data = data;
	q->front = q->rear = 0;

	return TRUE;
}
void DestroyQueue(SqQueue* q) {
	if (q == NULL) { return ; }
	xFree(q->rear);
}
uint8 QueueEmpty(SqQueue* q) {
	if (q == NULL) { return 1; }
	return (q->front == q->rear);
}
uint8 enQueue(SqQueue* q, QueueDateType e) {
	if ((q->rear + 1) % q->maxVal == q->front) {
		return FALSE;
	}
	q->rear = (q->rear + 1) % q->maxVal;
	q->data[q->rear] = e;
	return TRUE;
}
uint8 deQueue(SqQueue* q, QueueDateType *e) {
	if (q->front == q->rear) {
		return FALSE;
	}
	q->front = (q->front + 1) % q->maxVal;
	*e = q->data[q->front];
	return TRUE;
}


