
#include "msg.h"
#include "gui.h"
#include "SqQueue.h"
#include "x_malloc.h"

/*队列最大长度*/
#define MSG_EVENT_MAX_VAL	10000

/*事件队列*/
SqQueue eventMsg;
/*重绘队列*/
SqQueue drawMsg;

/*
* GUI初始化
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
/*获取消息*/
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
/*发送消息到队列*/
int8 GUISendTouchMsg(int ID,int16 x, int16 y) {
	HMSGE hMsg;
	hMsg = xMalloc(sizeof(MSGE));
	if (hMsg == NULL) { return FALSE; }

	hMsg->msgSrc = NULL;
	hMsg->msgType = MSG_TOUCH;
	hMsg->msgID = ID;
	hMsg->msgVal.xy.x = x;
	hMsg->msgVal.xy.y = y;
	hMsg->msgVal.xy.w = 0;
	hMsg->msgVal.xy.h = 0;

	enQueue(&eventMsg, hMsg);
	//GUIPostEvent(&Msg);
	return TRUE;
}
/*发送GUI消息*/
int8 GUISendMsg(void* hWin, uint8 msgType,uint8 msgID,int16 x,int16 y,uint16 w,uint16 h) {
	HMSGE hMsg;
	//if (hWin == NULL) { return FALSE; }

	hMsg = xMalloc(sizeof(MSGE));
	if (hMsg == NULL) { return FALSE; }


	hMsg->msgSrc = hWin;
	hMsg->msgType = msgType;
	hMsg->msgID = msgID;
	hMsg->msgVal.xy.x = x;
	hMsg->msgVal.xy.y = y;
	hMsg->msgVal.xy.w = w;
	hMsg->msgVal.xy.h = h;
	enQueue(&eventMsg, hMsg);

	//GUIPostEvent(&Msg);
	return TRUE;
}
int GUISendDrawMsg(void* hWin, uint8 msgType, uint8 msgID, int16 x, int16 y, uint16 w, uint16 h) {
	HMSGE hMsg;
	//if (hWin == NULL) { return FALSE; }

	hMsg = xMalloc(sizeof(MSGE));
	if (hMsg == NULL) { return FALSE; }


	hMsg->msgSrc = hWin;
	hMsg->msgType = msgType;
	hMsg->msgID = msgID;
	hMsg->msgVal.xy.x = x;
	hMsg->msgVal.xy.y = y;
	hMsg->msgVal.xy.w = w;
	hMsg->msgVal.xy.h = h;
	enQueue(&drawMsg, hMsg);

	//GUIPostEvent(&Msg);
	return TRUE;
}

/*获取消息*/
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

