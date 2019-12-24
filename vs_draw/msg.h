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

typedef struct {
	void* msgSrc;//ÏûÏ¢Ô´
	uint8 msgType;
	uint8 msgID;
	union {
		uint32 what;
		void * v;
		struct {
			int16 x;
			int16 y;
		}xy;
	}msgVal;
}*HMSGE, MSGE;

int8 GUISendMsg(void* hWin, uint8 msgType, uint8 msgID, int16 x, int16 y);
int8 GUISendTouchMsg(int ID, int16 x, int16 y);

#endif
