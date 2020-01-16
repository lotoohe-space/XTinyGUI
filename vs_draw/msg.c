
#include "msg.h"
#include "gui.h"
#include "SqQueue.h"
#include "x_malloc.h"

/*������󳤶�*/
#define MSG_EVENT_MAX_VAL	256

/*�¼�����*/
SqQueue eventMsg;
/*�ػ����*/
SqQueue drawMsg;
/*�ƶ��¼�����*/

/*
* GUI��ʼ��
*/
uint8 GUIMsgEventInit(void) {
	uint8 res;
	res = InitQueue(&eventMsg, MSG_EVENT_MAX_VAL);
	if (!res) { return FALSE; }

	res = InitQueue(&drawMsg, MSG_EVENT_MAX_VAL);
	if (!res) { 
		DestroyQueue(&eventMsg); 
		return FALSE;
	}
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
	hMsg->msgVal.rect.x = x;
	hMsg->msgVal.rect.y = y;

	enQueue(&eventMsg, hMsg);
	//GUIPostEvent(&Msg);
	return TRUE;
}
int GUISendDrawMsg(void* hWin, uint8 msgType, uint8 msgID, int16 x, int16 y, uint16 w, uint16 h
//	,int16 x1,int16 y1,uint16 w1,uint16 h1
) {
	HMSGE hMsg;
	//if (hWin == NULL) { return FALSE; }

	hMsg = xMalloc(sizeof(MSGE));
	if (hMsg == NULL) { return FALSE; }


	hMsg->msgSrc = hWin;
	hMsg->msgType = msgType;
	hMsg->msgID = msgID;
	hMsg->msgVal.rect.x = x;
	hMsg->msgVal.rect.y = y;
	hMsg->msgVal.rect.w = w;
	hMsg->msgVal.rect.h = h;
	/*hMsg->msgVal1.rect.x = x1;
	hMsg->msgVal1.rect.y = y1;
	hMsg->msgVal1.rect.w = w1;
	hMsg->msgVal1.rect.h = h1;*/
	enQueue(&drawMsg, hMsg);

	//GUIPostEvent(&Msg);
	return TRUE;
}


/*��ȡ��Ϣ*/
HMSGE GUIGetDrawMsg(void) {
	QueueDateType e;
	if (deQueue(&drawMsg, &e)) {
		return  (HMSGE)e;
	}
	return NULL;
}
void GUIDelDrawMsg(HMSGE hMsg) {
	xFree(hMsg);
}

int GUISendMoveMsg(void* hWin, uint8 msgType, uint8 msgID, int16 x, int16 y) {
	HMSGE hMsg;
	//if (hWin == NULL) { return FALSE; }

	hMsg = xMalloc(sizeof(MSGE));
	if (hMsg == NULL) { return FALSE; }


	hMsg->msgSrc = hWin;
	hMsg->msgType = msgType;
	hMsg->msgID = msgID;
	hMsg->msgVal.rect.x = x;
	hMsg->msgVal.rect.y = y;

	enQueue(&eventMsg, hMsg);

	return TRUE;
}
