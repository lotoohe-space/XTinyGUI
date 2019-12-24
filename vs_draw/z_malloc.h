#ifndef _MEM_H__
#define _MEM_H__
#include "type.h"

//内存管理
//管理可以用于内存分配的内存块
struct mem_man {
	volatile uint8  busy;//忙标志位
	void *addr;//可分配的内存的地址
	uint16 *mmt;//内存管理表的指针
	uint32 size;//可分配的内存的大小
	uint32 mmt_size;//可分配的内存管理表的大小
};
/** 能分配的内存大小 */
#define ZMEM_BANK_MIN_SIZE 8
/** 管理的内存的大小 */
#define ZMEM_SIZE (1024*1024*10)
/** 内存管理表的大小 */
#define ZMMTS (ZMEM_SIZE/ZMEM_BANK_MIN_SIZE)

/** 能分配的内存大小 */
#define ZMEM_BANK_MIN_SIZE_1 8
/** 管理的内存的大小 */
#define ZMEM_SIZE_1 (960*1024)
/** 内存管理表的大小 */
#define ZMMTS_1 (ZMEM_SIZE_1/ZMEM_BANK_MIN_SIZE_1)

////////////////////////////////////////////////////
#define IRAM 0	//内部内存块
#define ERAM 1	//外部内存块ioki

#define MEM0 0

extern int mem_num;//内存分配区的个数

////////////////////////////////////////////////////


uint8 t_get_free(uint8 mem_select);

void *t_malloc(uint8 mem_select,uint32 mem_num);
uint32 t_free(uint8 mem_select, void *mem_pointer);

#endif
