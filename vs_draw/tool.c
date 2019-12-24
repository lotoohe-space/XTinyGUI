
#include "type.h"
#include "gui.h"


//内存拷贝
void TMemcpy(void *dst, void *src, uint32 len) {
	if (!dst || !src) { return; }
	for (; len;len--) {
		*(((char*)dst)++) = *(((char*)src)++);
	}
}

//获取字符串长度
uint32 TStrlen(char *str) {
	uint32 len;
	if (!str) { return 0; }
	for (len=0; str[len];len++) ;

	return len;
}

/**
	* 判断两矩形是否相交
	*/
int8 isCollsionWithRect(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
	//XRECT temp;
	//XRECT rRect;
	//temp.x = x2;
	//temp.y = y2;
	//temp.w = w2;
	//temp.h = h2;
	//_GetOverLapRect(x1,y1,w1,h1, (&temp));
	//if (rRect.w == 0) {
	//	return FALSE;
	//}
	//else {
	//	return TRUE;
	//}
	// 矩形A位于矩形B的右侧
	if (x1 >= x2 && x1 >= x2 + w2) {
		return 0;
		// 矩形A位于矩形B的左侧	
	}else if (x1 <= x2 && x1 + w1 <= x2) {
			return 0;
		// 矩形A位于矩形B的下侧		
	}else if (y1 >= y2 && y1 >= y2 + h2) {
			return 0;
		// 矩形A位于矩形B的上侧		
	}else if (y1 <= y2 && y1 + h1 <= y2) {
		return 0;	
	}
	// 不相交都不满足，那就是相交了
	return 1;
		
}

//获得两个矩形的重叠部分
BOOL  GetOverLapRect(HXRECT r1, HXRECT r2, HXRECT res) {
	//	assert(r1 != NULL && r2 != NULL && res!=NULL);

	res->x = MAX(r1->x, r2->x);
	res->y = MAX(r1->y, r2->y);
	res->w = MIN(r1->x + r1->w, r2->x + r2->w);
	res->h = MIN(r1->y + r1->h, r2->y + r2->h);
	if ((res->x > res->w) || (res->y > res->h)) {
		res->x = r1->x;
		res->y = r1->y;
		res->w = 0;
		res->h = 0;
		return FALSE;
	}
	else {
		res->w -= res->x; res->h -= res->y;
	}
	return TRUE;
}
