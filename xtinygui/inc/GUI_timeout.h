#ifndef _GUI_TIMEOUT_H__
#define _GUI_TIMEOUT_H__

#include "type.h"
#include "x_tool.h"
typedef void (*GUITimeoutCallback)(void *arg);

typedef struct
{
	GUITimeoutCallback timoutCallBackFun;
	void *arg;
	uint32_t timeoutPeriod;
	uint32_t timeoutTick;
	uint8_t flag; /*0:bit 是否被使用*/
} *p_guitime_out_t, guitime_out_t;

/*是否被使用*/
#define _SetTimeoutUse(a) (_SET_BIT(((a))->flag, 0))
#define _ClrTimeoutUse(a) (_CLR_BIT(((a))->flag, 0))
#define _GetTimeoutUse(a) (_GET_BIT(((a))->flag, 0))

/*是否开启定时器*/
#define _OpenTimeout(a) (_SET_BIT(((a))->flag, 1))
#define _CloseTimeout(a) (_CLR_BIT(((a))->flag, 1))
#define _GetTimeout(a) (_GET_BIT(((a))->flag, 1))

void GUITimeoutFree(p_guitime_out_t hGUITimeout);
p_guitime_out_t GUITimeoutCreate(uint16_t timeoutPeriod, void *arg, GUITimeoutCallback timoutCallBackFun);
void GUITimeoutOpen(p_guitime_out_t hGUITimeOut);
void GUITimeoutClose(p_guitime_out_t hGUITimeOut);

/*非用户使用*/
void GUITimeoutProcess(void);

#endif
