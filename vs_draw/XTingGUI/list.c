
#include "x_malloc.h"
#include "list.h"
#include <stdio.h>


//新建一个list
HLIST ListNew(void) {
	HLIST temp_lm;
	temp_lm = xMalloc(sizeof(LIST_TYPE));
	if (temp_lm == 0x00) {
		return 0x00;
	}
	temp_lm->next = 0x00;
	temp_lm->lnext = 0x00;
	temp_lm->val = 0x00;
	return temp_lm;
}
//删除一个节点
int ListDel(HLIST list_m) {
	if (!list_m) { return 0; }
	//if(list_m->next){return -1;}
	xFree(list_m);
	return 0;
}
//在list头部添加元素
int ListAddFirst(HLIST list_m, HLIST add_item) {
	HLIST temp_lm;
	if (!list_m) { return -1; }
	temp_lm = list_m;
	add_item->next = temp_lm->next;
	if (temp_lm->next != NULL) {
		temp_lm->next->lnext = add_item;
	}
	add_item->lnext = temp_lm;
	temp_lm->next = add_item;
	return 0;
}
//在list尾部中添加元素
int ListAddLast(HLIST list_m, HLIST add_item) {
	HLIST temp_lm;
	if (!list_m) { return -1; }
	temp_lm = list_m;
	for (; temp_lm->next;) {
		temp_lm = temp_lm->next;
	}
	temp_lm->next = add_item;
	add_item->lnext = temp_lm;
	return 0;
}
/*获取最后一个元素*/
HLIST ListGetLast(HLIST list_m) {
	HLIST temp_lm;
	int len = 0;
	if (!list_m) { return 0x00; }
	temp_lm = list_m;
	for (; temp_lm->next;) {

		temp_lm = temp_lm->next;
	}
	return temp_lm;
}
/*获取第index个元素*/
HLIST ListGet(HLIST list_m, int index) {
	HLIST temp_lm;
	int len = 0;
	if (!list_m) { return 0x00; }
	temp_lm = list_m->next;
	for (; temp_lm;) {
		if (len == index) {
			return temp_lm;
		}
		len++;
		temp_lm = temp_lm->next;
	}
	return 0x00;
}
/*删除指定index元素*/
void ListDelInx(HLIST list_m, int index) {
	HLIST temp_lm;
	HLIST back_lm;
	int len = 0;
	if (!list_m) { return; }
	back_lm = list_m;
	temp_lm = list_m->next;
	for (; temp_lm;) {
		if (len == index) {
			//找到第index个元素
			HLIST cur_lm;
			cur_lm = temp_lm;//保存当前元素
			back_lm->next = temp_lm->next;//接起来
			xFree(cur_lm);//删除
		}
		len++;
		back_lm = temp_lm;
		temp_lm = temp_lm->next;
	}
}
/*通过Val值删除一个元素*/
int ListDelByVal(HLIST hList, void* val) {
	HLIST temp_lm;
	HLIST back_lm;
	int len = 0;
	if (!hList) { return FALSE; }
	back_lm = hList;
	temp_lm = hList->next;
	while (temp_lm) {
		if (temp_lm->val == val) {
			/*找到了这个元素，删除它*/
			back_lm->next = temp_lm->next;
			xFree(temp_lm);
			return TRUE;
		}
		back_lm = temp_lm;
		temp_lm = temp_lm->next;
	}
	return FALSE;
}
/*清空list*/
void ListClear(HLIST list_m) {
	HLIST temp_list_m = list_m;
	HLIST temp_lm;
	HLIST temp_back = list_m;

	for (; temp_list_m->next;) {
		temp_lm = temp_list_m;
		for (; temp_lm->next;) {
			temp_back = temp_lm;
			temp_lm = temp_lm->next;
		}
		temp_back->next = 0x00;
		xFree(temp_lm);
	}
	return;
}
/*List的长度*/
int ListLen(HLIST list_m) {
	HLIST temp_lm;
	int len = 0;
	if (!list_m) { return -1; }
	temp_lm = list_m->next;
	for (; temp_lm;) {
		len++;
		temp_lm = temp_lm->next;
	}
	return len;
}
