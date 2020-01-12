#ifndef _MSG_H__
#define _MSG_H__

#include "type.h"

#define MSG_TOUCH	0
#define MSG_KEY		1
#define MSG_WIN		2

#define MSG_TOUCH_PRESS 0
#define MSG_TOUCH_MOVE 1
#define MSG_TOUCH_RELEASE 2

#define MSG_WIN_MOVE 3 
#define MSG_WIN_INVAILD_UPDATE 4

typedef struct {
	void* msgSrc;//消息源
	uint8 msgType;
	uint8 msgID;
	union {
		uint32 what;
		void * v;
		/*设置消息的宽高*/
		struct {
			int16	x;
			int16	y;
			/*宽高*/
			uint16	w;
			uint16	h;
		}xy;
	}msgVal;
	/*偏移值*/
	struct {
		int16 dx;
		int16 dy;
	}dXY;
}*HMSGE, MSGE;

uint8 GUIMsgEventInit(void);
HMSGE GUIGetMsg(void);
void GUIDelMsg(HMSGE hMsg);
int8 GUISendMsg(void* hWin, uint8 msgType, uint8 msgID, int16 x, int16 y, uint16 w, uint16 h);
int8 GUISendTouchMsg(int ID, int16 x, int16 y);

int GUISendDrawMsg(void* hWin, uint8 msgType, uint8 msgID, int16 x, int16 y, uint16 w, uint16 h,int16 dx,int16 dy);
HMSGE GUIGetDrawMsg(void);
void GUIDelDrawMsg(HMSGE hMsg);

#endif
