
#include "SqQueue.h"

#include "x_malloc.h"


/*初始化队列*/
uint8 InitQueue(SqQueue *q,uint16 size){
	QueueDateType* data;
	if (q == NULL) { return FALSE; }
	data = xMalloc(sizeof(QueueDateType)*size);
	if (data == NULL) { return FALSE; }
	q->maxVal = size;
	q->data = data;
	q->front = q->rear = 0;
	q->valid = TRUE;
	return TRUE;
}
uint16 QueueLength(SqQueue* q) {
	if (q == NULL) { return 0; }
	return (q->rear - q->front + q->maxVal) % q->maxVal;
}
void DestroyQueue(SqQueue* q) {
	if (q == NULL) { return ; }
	xFree(q->data);
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


