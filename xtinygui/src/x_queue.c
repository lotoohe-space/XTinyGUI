
#include "x_queue.h"

#include "x_malloc.h"

/*初始化队列*/
uint8_t InitQueue(xqueue_t *q, uint16_t size)
{
	QueueDateType *data;
	if (q == NULL)
	{
		return FALSE;
	}
	data = xMalloc(sizeof(QueueDateType) * size);
	if (data == NULL)
	{
		return FALSE;
	}
	q->maxVal = size;
	q->data = data;
	q->front = q->rear = 0;
	q->valid = TRUE;
	return TRUE;
}
uint16_t QueueLength(xqueue_t *q)
{
	if (q == NULL)
	{
		return 0;
	}
	return (q->rear - q->front + q->maxVal) % q->maxVal;
}
void DestroyQueue(xqueue_t *q)
{
	if (q == NULL)
	{
		return;
	}
	xFree(q->data);
}
uint8_t QueueEmpty(xqueue_t *q)
{
	if (q == NULL)
	{
		return 1;
	}
	return (q->front == q->rear);
}
uint8_t enQueue(xqueue_t *q, QueueDateType e)
{
	if ((q->rear + 1) % q->maxVal == q->front)
	{
		return FALSE;
	}
	q->rear = (q->rear + 1) % q->maxVal;
	q->data[q->rear] = e;
	return TRUE;
}
uint8_t deQueue(xqueue_t *q, QueueDateType *e)
{
	if (q->front == q->rear)
	{ /*空了，则返回错误*/
		return FALSE;
	}
	q->front = (q->front + 1) % q->maxVal;
	*e = q->data[q->front];
	return TRUE;
}
/*获取队尾的元素*/
uint8_t getTailQueue(xqueue_t *q, QueueDateType *e)
{
	if (q->front == q->rear)
	{ /*空了，则返回错误*/
		return FALSE;
	}
	*e = q->data[(q->front + 1) % q->maxVal];
	return TRUE;
}
