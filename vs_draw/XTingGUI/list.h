#ifndef _MUSCI_LIST_H__
#define _MUSCI_LIST_H__


//////////////ָ������///////////////////////////
#define LIST_TYPE struct list //��������
typedef LIST_TYPE* LIST_POINTER; //�б�ָ��
/////////////////////////////////////////////////////

typedef LIST_TYPE{
	void * val;
	LIST_POINTER next;//��ڵ�
	LIST_POINTER lnext;//�ҽڵ�
}*HLIST,LIST;

HLIST ListNew(void);
int ListDel(HLIST list_m);
int ListAddFirst(HLIST list_m, HLIST add_item);
int ListAddLast(HLIST list_m, HLIST add_item);
HLIST ListGet(HLIST list_m, int index);
void ListDelInx(HLIST list_m, int index);
void ListClear(HLIST list_m);
int ListLen(HLIST list_m);
HLIST ListGetLast(HLIST list_m);

#define _ListItemGetValU(a) (a)->val
#define _ListItemNextU(a) (a)=(a)->next
#define _StartScanU(a) {HLIST tempItem=a;\
	(tempItem) = (tempItem)->next;\
	while (tempItem) {\
		void *val=_ListItemGetValU(tempItem);

#define _EndScanU()  _ListItemNextU(tempItem);}}
	
	

#endif




