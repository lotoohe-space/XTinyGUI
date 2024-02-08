
#include "x_malloc.h"
#include "list.h"
#include <stdio.h>


//�½�һ��list
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
//ɾ��һ���ڵ�
int ListDel(HLIST list_m) {
	if (!list_m) { return 0; }
	//if(list_m->next){return -1;}
	xFree(list_m);
	return 0;
}
//��listͷ�����Ԫ��
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
//��listβ�������Ԫ��
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
/*��ȡ���һ��Ԫ��*/
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
/*��ȡ��index��Ԫ��*/
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
/*ɾ��ָ��indexԪ��*/
void ListDelInx(HLIST list_m, int index) {
	HLIST temp_lm;
	HLIST back_lm;
	int len = 0;
	if (!list_m) { return; }
	back_lm = list_m;
	temp_lm = list_m->next;
	for (; temp_lm;) {
		if (len == index) {
			//�ҵ���index��Ԫ��
			HLIST cur_lm;
			cur_lm = temp_lm;//���浱ǰԪ��
			back_lm->next = temp_lm->next;//������
			xFree(cur_lm);//ɾ��
		}
		len++;
		back_lm = temp_lm;
		temp_lm = temp_lm->next;
	}
}
/*ͨ��Valֵɾ��һ��Ԫ��*/
int ListDelByVal(HLIST hList, void* val) {
	HLIST temp_lm;
	HLIST back_lm;
	int len = 0;
	if (!hList) { return FALSE; }
	back_lm = hList;
	temp_lm = hList->next;
	while (temp_lm) {
		if (temp_lm->val == val) {
			/*�ҵ������Ԫ�أ�ɾ����*/
			back_lm->next = temp_lm->next;
			xFree(temp_lm);
			return TRUE;
		}
		back_lm = temp_lm;
		temp_lm = temp_lm->next;
	}
	return FALSE;
}
/*���list*/
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
/*List�ĳ���*/
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
