#ifndef _MEM_H__
#define _MEM_H__
#include "type.h"

//�ڴ����
//������������ڴ������ڴ��
struct mem_man {
	volatile uint8  busy;//æ��־λ
	void *addr;//�ɷ�����ڴ�ĵ�ַ
	uint16 *mmt;//�ڴ������ָ��
	uint32 size;//�ɷ�����ڴ�Ĵ�С
	uint32 mmt_size;//�ɷ�����ڴ�����Ĵ�С
};
/** �ܷ�����ڴ��С */
#define ZMEM_BANK_MIN_SIZE 8
/** ������ڴ�Ĵ�С */
#define ZMEM_SIZE (1024*1024*10)
/** �ڴ�����Ĵ�С */
#define ZMMTS (ZMEM_SIZE/ZMEM_BANK_MIN_SIZE)

/** �ܷ�����ڴ��С */
#define ZMEM_BANK_MIN_SIZE_1 8
/** ������ڴ�Ĵ�С */
#define ZMEM_SIZE_1 (960*1024)
/** �ڴ�����Ĵ�С */
#define ZMMTS_1 (ZMEM_SIZE_1/ZMEM_BANK_MIN_SIZE_1)

////////////////////////////////////////////////////
#define IRAM 0	//�ڲ��ڴ��
#define ERAM 1	//�ⲿ�ڴ��ioki

#define MEM0 0

extern int mem_num;//�ڴ�������ĸ���

////////////////////////////////////////////////////


uint8 t_get_free(uint8 mem_select);

void *t_malloc(uint8 mem_select,uint32 mem_num);
uint32 t_free(uint8 mem_select, void *mem_pointer);

#endif
