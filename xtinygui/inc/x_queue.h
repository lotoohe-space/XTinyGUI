#pragma once

#ifndef _SQQUEUE_H__
#define _SQQUEUE_H__
#include "type.h"

typedef void *QueueDateType;

typedef struct
{
	QueueDateType *data;
	int16_t front, rear;
	uint16_t maxVal;
	uint8_t valid;
} *p_xqueue_t, xqueue_t;

uint8_t InitQueue(xqueue_t *q, uint16_t size);
void DestroyQueue(xqueue_t *q);
uint8_t QueueEmpty(xqueue_t *q);
uint8_t enQueue(xqueue_t *q, QueueDateType e);
uint8_t deQueue(xqueue_t *q, QueueDateType *e);
uint8_t getTailQueue(xqueue_t *q, QueueDateType *e);

#endif
