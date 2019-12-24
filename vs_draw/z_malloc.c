#include "z_malloc.h"
#include <string.h>

//#include "task.h"
/** �ڴ����Ҫ8�ֽڶ��� */
//__align(8) 
uint8 z_mem_malloc[ZMEM_SIZE] = { 0 };		
//uint8 Z_mem_malloc1[ZMEM_SIZE_1]={0};		//�ⲿSRAM�ڴ��
////�ڴ���ƿ�
 uint16 z_mmt[ZMMTS] = { 0 };
// u16 z_mmt1[ZMMTS_1] __attribute__((at(0X68000000+ZMEM_SIZE_1)))= { 0 };

//�ڴ�����
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

//�ڴ�������ĸ���
int mem_num=sizeof(mem_man_list)/sizeof(struct mem_man);

void t_malloc_init(void){
	//����ڴ�����
	memset(z_mmt, 0, mem_man_list[0].mmt_size * 2);
	//memset(z_mmt1, 0, mem_man_list[1].mmt_size * 2);
}
//��ȡ������ڴ�
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
//�����ڴ棬�ú���������ȫ
void *t_malloc(uint8 mem_select,uint32 mem_num){
	static uint8 start_flag=0;
	volatile uint32 i;
	volatile uint32 j;
	uint8  suc_flag=0;
	volatile float bank_num_f;
	/** ������Ҫ������ٿ���ڴ� */
	volatile uint32 bank_num;
	//��һ��Ҫ���г�ʼ��
	if(!start_flag){
		start_flag=1;
		t_malloc_init();
	}
	//æ�ȴ�
	while (mem_man_list[mem_select].busy);
	mem_man_list[mem_select].busy = 1;

	bank_num_f	= (float)((double)mem_num/(double)ZMEM_BANK_MIN_SIZE);
	bank_num=bank_num_f>((uint32)bank_num_f)?((uint32)bank_num_f)+1:((uint32)bank_num_f);
	if (bank_num > mem_man_list[mem_select].mmt_size) { mem_man_list[mem_select].busy = 0; return 0x00000000L; }
	//enter_int();
	/** ������Ҫ�Ŀտ� */
	for(i=0;i<mem_man_list[mem_select].mmt_size;i++){
		uint32 valid=0;
		//����ҵ��˿տ飬���Ƿ���������ô��Ŀտ�
		if(mem_man_list[mem_select] .mmt[i]==0){
			for(j=i;j<i+bank_num&&j<mem_man_list[mem_select].mmt_size;j++){
				if(mem_man_list[mem_select].mmt[j]==0){valid++;}
				else{break;}//����������ڴ�飬���˳�
			}
			if(valid>=bank_num){
				uint32 n;
				//����ɹ���,���ñ��Ϊ1
				for(n=i;n<i+bank_num;n++){ mem_man_list[mem_select].mmt[n]=1;}
				//��һ���������ռ�õĴ�С
				mem_man_list[mem_select].mmt[i]=bank_num;
				//����Ϊ�ҵ���
				suc_flag=1;
				break;
			}
		}
	}
	//exit_int();
	if(suc_flag){
		mem_man_list[mem_select].busy = 0;
		//����ҵ��ɹ�,���ػ�ȡ���ĵ�ַ
		return (void*)((uint32)(mem_man_list[mem_select].addr) + i*ZMEM_BANK_MIN_SIZE);
	}
	mem_man_list[mem_select].busy = 0;
	return 0x00000000L;
}
//�ͷ��ڴ棬�ú���������ȫ
uint32 t_free(uint8 mem_select, void *mem_pointer){
	//�ҵ�ƫ�Ƶ�ַ
	uint32 offset;
	uint32 j=0;
	uint32 num;
	//Ϊ�����˳�
	if(!mem_pointer){return 0;}
	//æ�ȴ�
	while (mem_man_list[mem_select].busy);
	mem_man_list[mem_select].busy = 1;
	//�ҵ��ڴ���ƫ�Ƶ�ַ
	offset = (uint32)mem_pointer - (uint32)(mem_man_list[mem_select].addr);
	//�ҵ�������е�ƫ�ƣ����ͷ�
	offset/=ZMEM_BANK_MIN_SIZE;
	//enter_int();
	//���ָ����ڴ��еĿ�Ϊ0��˵�����ͷŹ��˻����ڴ淢����й¶
	if(!mem_man_list[mem_select].mmt[offset]){
		//exit_int();
		mem_man_list[mem_select].busy = 0;
		return 0;
	}
	num= mem_man_list[mem_select].mmt[offset];
	//���
	for(j=offset;j<offset+num;j++){ mem_man_list[mem_select].mmt[j]=0;}
	//exit_int();
	mem_man_list[mem_select].busy = 0;
	return 1;
}




