
#include "msg.h"
#include "gui.h"

MSGE Msg;
int8 GUISendTouchMsg(int ID,int16 x, int16 y) {
	Msg.msgSrc = NULL;
	Msg.msgType = MSG_TOUCH;
	Msg.msgID = ID;
	Msg.msgVal.xy.x = x;
	Msg.msgVal.xy.y = y;
	GUIPostEvent(&Msg);
	//GUIEvent(&Msg);
	return 0;
}
MSGE Msg1;
/*·¢ËÍGUIÏûÏ¢*/
int8 GUISendMsg(void* hWin, uint8 msgType,uint8 msgID,int16 x,int16 y) {
	if (hWin == NULL) {
		return -1;
	}
	Msg1.msgSrc = hWin;
	Msg1.msgType = msgType;
	Msg1.msgID = msgID;
	Msg1.msgVal.xy.x = x;
	Msg1.msgVal.xy.y = y;
	GUIPostEvent(&Msg1);

	return 0;
}

