
#include "msg.h"
#include "gui.h"
#include "SqQueue.h"
#include "x_malloc.h"

/*队列最大长度*/
#define MSG_EVENT_MAX_VAL	64

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
	eventMsg.valid = FALSE;
	drawMsg.valid = FALSE;

	return TRUE;
}
void GUIEventValid(void) {
	eventMsg.valid = TRUE;
	drawMsg.valid = TRUE;
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
int8 GUISendKeyMsg(uint8 ID,uint8 status) {
	HMSGE hMsg;
	if (eventMsg.valid == FALSE) { return FALSE; }
	hMsg = xMalloc(sizeof(MSGE));
	if (hMsg == NULL) { return FALSE; }

	hMsg->msgSrc = NULL;
	hMsg->msgType = MSG_KEY;
	hMsg->msgID = ID;
	hMsg->msgVal.status = status;
	//hMsg->msgVal.what = keyVal;

	enQueue(&eventMsg, hMsg);
	//GUIPostEvent(&Msg);
	return TRUE;
}
/*发送消息到队列*/
int8 GUISendTouchMsg(int ID,int16 x, int16 y) {
	HMSGE hMsg;
	if (eventMsg.valid == FALSE) { return FALSE; }
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
	if (drawMsg.valid == FALSE) { return FALSE; }
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

int GUISendMoveMsg(void* hWin, uint8 msgType, uint8 msgID, int16 x, int16 y) {
	HMSGE hMsg;
	//if (hWin == NULL) { return FALSE; }
	if (eventMsg.valid == FALSE) { return FALSE; }
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
