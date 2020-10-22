
#include "msg.h"
#include "gui.h"
#include "SqQueue.h"
#include "x_malloc.h"

/*������󳤶�*/
#define MSG_EVENT_MAX_VAL	64

/*�¼�����*/
SqQueue eventMsg;
/*�ػ����*/
SqQueue drawMsg;
/*�����ƶ�����*/
SqQueue moveMsg;
/*ɾ��*/
SqQueue winDelMsg;
/*����ƶ�*/
SqQueue cursorMsg;

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
	res = InitQueue(&moveMsg, MSG_EVENT_MAX_VAL);
	if (!res) {
		DestroyQueue(&eventMsg);
		DestroyQueue(&drawMsg);
		return FALSE;
	}
	res = InitQueue(&winDelMsg, MSG_EVENT_MAX_VAL);
	if (!res) {
		DestroyQueue(&eventMsg);
		DestroyQueue(&drawMsg);
		DestroyQueue(&moveMsg);
		return FALSE;
	}
	res = InitQueue(&cursorMsg, MSG_EVENT_MAX_VAL);
	if (!res) {
		DestroyQueue(&eventMsg);
		DestroyQueue(&drawMsg);
		DestroyQueue(&moveMsg);
		DestroyQueue(&winDelMsg);
		return FALSE;
	}
	eventMsg.valid = FALSE;
	drawMsg.valid = FALSE;
	moveMsg.valid = FALSE;
	winDelMsg.valid = FALSE;
	cursorMsg.valid = FALSE;

	return TRUE;
}
void GUIEventValid(void) {
	eventMsg.valid = TRUE;
	drawMsg.valid = TRUE;
	moveMsg.valid = TRUE;
	winDelMsg.valid = TRUE;
	cursorMsg.valid = TRUE;
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

/*������Ϣ������*/
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
	if (moveMsg.valid == FALSE) { return FALSE; }
	hMsg = xMalloc(sizeof(MSGE));
	if (hMsg == NULL) { return FALSE; }


	hMsg->msgSrc = hWin;
	hMsg->msgType = msgType;
	hMsg->msgID = msgID;
	hMsg->msgVal.rect.x = x;
	hMsg->msgVal.rect.y = y;

	enQueue(&moveMsg, hMsg);

	return TRUE;
}
HMSGE GUIGetMoveMsg(void) {
	QueueDateType e,e1;
next:	
	if (deQueue(&moveMsg, &e)) {
		if (getTailQueue(&moveMsg, &e1) == FALSE) {
			return  (HMSGE)e;
		}
		if (((HMSGE)e)->msgSrc == ((HMSGE)e1)->msgSrc) {
			goto next;
		}
		else {
			return (HMSGE)e;
		}
	}

	return NULL;
}
void GUIDelMoveMsg(HMSGE hMsg) {
	xFree(hMsg);
}
int8 GUISendWINDelMsg(void* msgSrc, void* delItem) {
	HMSGE hMsg;
	if (winDelMsg.valid == FALSE) { return FALSE; }
	hMsg = xMalloc(sizeof(MSGE));
	if (hMsg == NULL) { return FALSE; }
	hMsg->msgSrc = msgSrc;
	hMsg->msgType = MSG_WIN;
	hMsg->msgID = MSG_WIN_DEL_WIDGE;
	hMsg->msgVal.v = delItem;

	enQueue(&winDelMsg, hMsg);
	return TRUE;
}
/*��ȡ��Ϣ*/
HMSGE GUIGetWINDelMsg(void) {
	QueueDateType e;
	if (deQueue(&winDelMsg, &e)) {
		return  (HMSGE)e;
	}

	return NULL;
}
void GUIDelWinDelMsg(HMSGE hMsg) {
	xFree(hMsg);
}

int8 GUISendCursorMsg(uint16 ID, int16 x, int16 y) {
	HMSGE hMsg;
	if (cursorMsg.valid == FALSE) { return FALSE; }
	hMsg = xMalloc(sizeof(MSGE));
	if (hMsg == NULL) { return FALSE; }

	hMsg->msgSrc = NULL;
	hMsg->msgType = MSG_CURSOR;
	hMsg->msgID = ID;
	hMsg->msgVal.rect.x = x;
	hMsg->msgVal.rect.y = y;
	//hMsg->msgVal.what = keyVal;

	enQueue(&cursorMsg, hMsg);
	//GUIPostEvent(&Msg);
	return TRUE;
}
void GUIDelCursorMsg(HMSGE hMsg) {
	xFree(hMsg);
}
/*��ȡ��Ϣ*/
HMSGE GUIGetCursorMsg(void) {
	QueueDateType e;
	if (deQueue(&cursorMsg, &e)) {
		return  (HMSGE)e;
	}

	return NULL;
}