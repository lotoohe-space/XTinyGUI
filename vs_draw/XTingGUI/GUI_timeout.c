#include "GUI_timeout.h"
#include "x_malloc.h"
#include "gui.h"

#define GUI_TIMEOUT_MAX_COUNT 10
GUITIME_OUT GUITimeoutList[GUI_TIMEOUT_MAX_COUNT] = { 0 };

PRIVATE HGUITIME_OUT GUITimeoutNew(void) {
	uint16 i = 0;
	for (i = 0; i < GUI_TIMEOUT_MAX_COUNT; i++) {
		if (!_GetTimeoutUse(&GUITimeoutList[i])) {
			_SetTimeoutUse(&GUITimeoutList[i]);
			return &GUITimeoutList[i];
		}
	}
	return NULL;
}
PUBLIC void GUITimeoutFree(HGUITIME_OUT hGUITimeout) {
	if (hGUITimeout == NULL) { return; }
	_ClrTimeoutUse(hGUITimeout);
}

HGUITIME_OUT GUITimeoutCreate(uint16 timeoutPeriod, void* arg, GUITimeoutCallback timoutCallBackFun) {
	HGUITIME_OUT hGUITimeOut = GUITimeoutNew();
	if (hGUITimeOut == NULL) { return NULL; }


	hGUITimeOut->timeoutTick = GUIGetTick();
	hGUITimeOut->timeoutPeriod = timeoutPeriod;
	hGUITimeOut->timoutCallBackFun = timoutCallBackFun;
	hGUITimeOut->arg = arg;
	_OpenTimeout(hGUITimeOut);
	return hGUITimeOut;
}
void GUITimeoutOpen(HGUITIME_OUT hGUITimeOut) {
	if (hGUITimeOut == NULL) { return; }
	hGUITimeOut->timeoutTick = GUIGetTick();
	_OpenTimeout(hGUITimeOut);
}
void GUITimeoutClose(HGUITIME_OUT hGUITimeOut) {
	if (hGUITimeOut == NULL) { return; }
	_CloseTimeout(hGUITimeOut);
}
void GUITimeoutProcess(void) {
	uint16 i = 0;
	for (i = 0; i < GUI_TIMEOUT_MAX_COUNT; i++) {
		if (_GetTimeoutUse(&GUITimeoutList[i]) && _GetTimeout(&GUITimeoutList[i])) {
			if (GUIGetTick() - GUITimeoutList[i].timeoutTick >= GUITimeoutList[i].timeoutPeriod) {
				_CloseTimeout(&GUITimeoutList[i]);
				GUITimeoutList[i].timoutCallBackFun(GUITimeoutList[i].arg);
			}
		}
	}
}
