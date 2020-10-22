#ifndef _GUI_TIMEOUT_H__
#define _GUI_TIMEOUT_H__

#include "type.h"
#include "tool.h"
typedef void (*GUITimeoutCallback)(void* arg);

typedef struct {
	GUITimeoutCallback	timoutCallBackFun;
	void*				arg;
	uint32				timeoutPeriod;
	uint32				timeoutTick;
	uint8				flag;				/*0:bit �Ƿ�ʹ��*/
}*HGUITIME_OUT, GUITIME_OUT;

/*�Ƿ�ʹ��*/
#define _SetTimeoutUse(a)	(_SET_BIT(((a))->flag,0))
#define _ClrTimeoutUse(a)	(_CLR_BIT(((a))->flag,0))
#define _GetTimeoutUse(a)	(_GET_BIT(((a))->flag,0))

/*�Ƿ�����ʱ��*/
#define _OpenTimeout(a)		(_SET_BIT(((a))->flag,1))
#define _CloseTimeout(a)	(_CLR_BIT(((a))->flag,1))
#define _GetTimeout(a)		(_GET_BIT(((a))->flag,1))

void			GUITimeoutFree(HGUITIME_OUT hGUITimeout);
HGUITIME_OUT	GUITimeoutCreate(uint16 timeoutPeriod, void* arg, GUITimeoutCallback timoutCallBackFun);
void			GUITimeoutOpen(HGUITIME_OUT hGUITimeOut);
void			GUITimeoutClose(HGUITIME_OUT hGUITimeOut);

/*���û�ʹ��*/
void			GUITimeoutProcess(void);

#endif

