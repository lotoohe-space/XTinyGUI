#ifndef _TABLE_VIEW_WIDGET_H__
#define _TABLE_VIEW_WIDGET_H__

#include "type.h"
#include "group_widge.h"

#define TABLE_WIDGE_MARK_HEAD(a)	TableWidge##a

#define TABLE_WIDGE_H_NUM			5
#define TABLE_WIDGE_V_NUM			5

#define TABLE_WIDGE_HEIGHT			30
#define TABLE_WIDGE_WIDTH			30

#define TABLE_WIDGE_ITEM_GAP		5

typedef struct {
	GROUP_WIDGE		groupWidge;		/*�̳�GROUP_WIDGE*/

	uint8			HItemsNum;		/*������ٿؼ�*/
	uint8			VItemsNum;		/*������ٿؼ�*/
	uint16			itemHeight;		/*�ؼ��߶�*/
	uint16			itemWidth;		/*�ؼ����*/

	uint8			flag;		
}*HTABLE_WIDGE, TABLE_WIDGE;


#endif
