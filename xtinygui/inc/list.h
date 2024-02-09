#ifndef _MUSCI_LIST_H__
#define _MUSCI_LIST_H__

//////////////指针类型///////////////////////////
#define LIST_TYPE struct list	 // 链表类型
typedef LIST_TYPE *LIST_POINTER; // 列表指针
/////////////////////////////////////////////////////

typedef LIST_TYPE
{
	void *val;
	LIST_POINTER next;	// 左节点
	LIST_POINTER lnext; // 右节点
}
*p_rlist_t, list_t;

p_rlist_t ListNew(void);
int ListDel(p_rlist_t list_m);
int ListAddFirst(p_rlist_t list_m, p_rlist_t add_item);
int ListAddLast(p_rlist_t list_m, p_rlist_t add_item);
p_rlist_t ListGet(p_rlist_t list_m, int index);
/*通过Val值删除一个元素*/
int ListDelByVal(p_rlist_t hList, void *val);
void ListDelInx(p_rlist_t list_m, int index);
void ListClear(p_rlist_t list_m);
int ListLen(p_rlist_t list_m);
p_rlist_t ListGetLast(p_rlist_t list_m);

#define _ListItemGetValU(a) (a)->val
#define _ListItemNextU(a) (a) = (a)->next
#define _StartScanU(a)                 \
	{                                  \
		p_rlist_t tempItem = a;        \
		(tempItem) = (tempItem)->next; \
		while (tempItem)               \
		{                              \
			void *val = _ListItemGetValU(tempItem);

#define _EndScanU()           \
	_ListItemNextU(tempItem); \
	}                         \
	}

#endif
