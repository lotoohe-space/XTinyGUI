#include "z_malloc.h"
#include <string.h>

//#include "task.h"
/** 内存必须要8字节对齐 */
//__align(8) 
uint8 z_mem_malloc[ZMEM_SIZE] = { 0 };		
//uint8 Z_mem_malloc1[ZMEM_SIZE_1]={0};		//外部SRAM内存池
////内存控制块
uint16 z_mmt[ZMMTS] = { 0 };
// u16 z_mmt1[ZMMTS_1] __attribute__((at(0X68000000+ZMEM_SIZE_1)))= { 0 };

//内存管理表
struct mem_man mem_man_list[] = {
	{
		0,
		z_mem_malloc,
		z_mmt,
		ZMEM_SIZE,
		ZMMTS
	},
	//{
	//	0,
	//	Z_mem_malloc1,
	//	z_mmt1,
	//	ZMEM_SIZE_1,
	//	ZMMTS_1
	//}
};

//内存分配区的个数
int mem_num=sizeof(mem_man_list)/sizeof(struct mem_man);

void t_malloc_init(void){
	//清空内存管理表
	memset(z_mmt, 0, mem_man_list[0].mmt_size * 2);
	//memset(z_mmt1, 0, mem_man_list[1].mmt_size * 2);
}
//获取空余的内存
uint8 t_get_free(uint8 mem_select){
	uint32 i;
	uint32 j=0;
	for(i=0;i<mem_man_list[mem_select].mmt_size;i++){
		if(mem_man_list[mem_select].mmt[i]==0x00){
			j++;
		}
	}
	return (uint8)(((float)j)/((float)mem_man_list[mem_select].mmt_size)*100);
}
//申请内存，该函数多任务安全
void *t_malloc(uint8 mem_select,uint32 mem_num){
	static uint8 start_flag=0;
	volatile uint32 i;
	volatile uint32 j;
	uint8  suc_flag=0;
	volatile float bank_num_f;
	/** 任务需要分配多少块的内存 */
	volatile uint32 bank_num;
	//第一次要进行初始化
	if(!start_flag){
		start_flag=1;
		t_malloc_init();
	}
	//忙等待
	while (mem_man_list[mem_select].busy);
	mem_man_list[mem_select].busy = 1;

	bank_num_f	= (float)((double)mem_num/(double)ZMEM_BANK_MIN_SIZE);
	bank_num=bank_num_f>((uint32)bank_num_f)?((uint32)bank_num_f)+1:((uint32)bank_num_f);
	if (bank_num > mem_man_list[mem_select].mmt_size) { mem_man_list[mem_select].busy = 0; return 0x00000000L; }
	//enter_int();
	/** 查找需要的空块 */
	for(i=0;i<mem_man_list[mem_select].mmt_size;i++){
		uint32 valid=0;
		//如果找到了空块，则看是否有连续这么多的空块
		if(mem_man_list[mem_select] .mmt[i]==0){
			for(j=i;j<i+bank_num&&j<mem_man_list[mem_select].mmt_size;j++){
				if(mem_man_list[mem_select].mmt[j]==0){valid++;}
				else{break;}//如果其中有内存块，则退出
			}
			if(valid>=bank_num){
				uint32 n;
				//申请成功了,设置标记为1
				for(n=i;n<i+bank_num;n++){ mem_man_list[mem_select].mmt[n]=1;}
				//第一个管理格存放占用的大小
				mem_man_list[mem_select].mmt[i]=bank_num;
				//设置为找到了
				suc_flag=1;
				break;
			}
		}
	}
	//exit_int();
	if(suc_flag){
		mem_man_list[mem_select].busy = 0;
		//如果找到成功,返回获取到的地址
		return (void*)((uint32)(mem_man_list[mem_select].addr) + i*ZMEM_BANK_MIN_SIZE);
	}
	mem_man_list[mem_select].busy = 0;
	return 0x00000000L;
}
//释放内存，该函数多任务安全
uint32 t_free(uint8 mem_select, void *mem_pointer){
	//找到偏移地址
	uint32 offset;
	uint32 j=0;
	uint32 num;
	//为空则退出
	if(!mem_pointer){return 0;}
	//忙等待
	while (mem_man_list[mem_select].busy);
	mem_man_list[mem_select].busy = 1;
	//找到内存块的偏移地址
	offset = (uint32)mem_pointer - (uint32)(mem_man_list[mem_select].addr);
	//找到管理块中的偏移，并释放
	offset/=ZMEM_BANK_MIN_SIZE;
	//enter_int();
	//如果指向的内存中的块为0，说明被释放过了或者内存发生了泄露
	if(!mem_man_list[mem_select].mmt[offset]){
		//exit_int();
		mem_man_list[mem_select].busy = 0;
		return 0;
	}
	num= mem_man_list[mem_select].mmt[offset];
	//清空
	for(j=offset;j<offset+num;j++){ mem_man_list[mem_select].mmt[j]=0;}
	//exit_int();
	mem_man_list[mem_select].busy = 0;
	return 1;
}




