
#include "msg.h"
#include "gui.h"
#include "SqQueue.h"
#include "x_malloc.h"

/*������󳤶�*/
#define MSG_EVENT_MAX_VAL	256

/*�¼�����*/
SqQueue eventMsg;

/*
* GUI��ʼ��
*/
uint8 GUIMsgEventInit(void) {
	uint8 res;
	res = InitQueue(&eventMsg, MSG_EVENT_MAX_VAL);
	if (!res) { return FALSE; }

	return TRUE;
}
/*��ȡ��Ϣ*/
HMSGE GUIGetMsg(void) {
	QueueDateType e;
	if (deQueue(&eventMsg, &e)) {
		return  (HMSGE)e;
	}
	return NULL;
}
void GUIDelMsg(HMSGE hMsg) {
	xFree(hMsg);
}
/*������Ϣ������*/
int8 GUISendTouchMsg(int ID,int16 x, int16 y) {
	HMSGE hMsg;
	hMsg = xMalloc(sizeof(MSGE));
	if (hMsg == NULL) { return FALSE; }

	hMsg->msgSrc = NULL;
	hMsg->msgType = MSG_TOUCH;
	hMsg->msgID = ID;
	hMsg->msgVal.xy.x = x;
	hMsg->msgVal.xy.y = y;

	enQueue(&eventMsg, hMsg);
	//GUIPostEvent(&Msg);
	return TRUE;
}
/*����GUI��Ϣ*/
int8 GUISendMsg(void* hWin, uint8 msgType,uint8 msgID,int16 x,int16 y) {
	HMSGE hMsg;
	if (hWin == NULL) { return FALSE; }

	hMsg = xMalloc(sizeof(MSGE));
	if (hMsg == NULL) { return FALSE; }


	hMsg->msgSrc = hWin;
	hMsg->msgType = msgType;
	hMsg->msgID = msgID;
	hMsg->msgVal.xy.x = x;
	hMsg->msgVal.xy.y = y;
	enQueue(&eventMsg, hMsg);

	//GUIPostEvent(&Msg);
	return TRUE;
}

