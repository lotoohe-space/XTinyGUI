#pragma once

#ifndef _SQQUEUE_H__
#define _SQQUEUE_H__
#include "type.h"

typedef void* QueueDateType;

typedef struct {
	QueueDateType* data;
	int16 front, rear;
	uint16 maxVal;
	uint8 valid;
}*PSqQueue, SqQueue;

uint8 InitQueue(SqQueue* q, uint16 size);
void DestroyQueue(SqQueue* q);
uint8 QueueEmpty(SqQueue* q);
uint8 enQueue(SqQueue* q, QueueDateType e);
uint8 deQueue(SqQueue* q, QueueDateType* e);

#endif

