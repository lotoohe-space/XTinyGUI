#ifndef _MUSCI_LIST_H__
#define _MUSCI_LIST_H__


//////////////指针类型///////////////////////////
#define LIST_TYPE struct list //链表类型
typedef LIST_TYPE* LIST_POINTER; //列表指针
/////////////////////////////////////////////////////

typedef LIST_TYPE{
	void *			val;
	LIST_POINTER	next;//左节点
	LIST_POINTER	lnext;//右节点
}*HLIST,LIST;

HLIST	ListNew(void);
int		ListDel(HLIST list_m);
int		ListAddFirst(HLIST list_m, HLIST add_item);
int		ListAddLast(HLIST list_m, HLIST add_item);
HLIST	ListGet(HLIST list_m, int index);
/*通过Val值删除一个元素*/
int		ListDelByVal(HLIST hList, void* val);
void	ListDelInx(HLIST list_m, int index);
void	ListClear(HLIST list_m);
int		ListLen(HLIST list_m);
HLIST	ListGetLast(HLIST list_m);

#define _ListItemGetValU(a) (a)->val
#define _ListItemNextU(a) (a)=(a)->next
#define _StartScanU(a) {HLIST tempItem=a;\
	(tempItem) = (tempItem)->next;\
	while (tempItem) {\
		void *val=_ListItemGetValU(tempItem);

#define _EndScanU()  _ListItemNextU(tempItem);}}
	
	

#endif




