
#include "type.h"
#include "gui.h"


//�ڴ濽��
void TMemcpy(void *dst, void *src, uint32 len) {
	if (!dst || !src) { return; }
	for (; len;len--) {
		*(((char*)dst)++) = *(((char*)src)++);
	}
}
//��ȡgbk�ַ����ĵ�index���ַ��ĵ�ַ
const char* TCharGet(const char* str, uint16 index) {
	uint16 i = 0;
	uint16 len = 0;
	if (!str) { return NULL; }
	while (str[i]) {
		if (len == index) {
			return &str[i];
		}
		len++;
		if (str[i] & 0x80) {
			i += 2;
		}
		else {
			i++;
		}
	}
	return NULL;
}
uint32 UStrlen(uint16* str) {
	uint16 i = 0;
	if (str == NULL) { return 0; }
	while (str[i]){ 
		i++; 
	}
	return i;
}
//��ȡGBK�ַ�������
uint32 TStrlen(const char *str) {
	uint16 i = 0;
	uint16 len = 0;
	if (!str) { return 0; }
	while (str[i]) {
		if (str[i] & 0x80) {
			i += 2;
		}
		else {
			i++;	
		}
		len++;
	}

	return len;
}

/**
	* �ж��������Ƿ��ཻ
	*/
int8 _INLINE_ isCollsionWithRect(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
	// ����Aλ�ھ���B���Ҳ�
	if (x1 >= x2 && x1 >= x2 + w2) {
		return FALSE;
		// ����Aλ�ھ���B�����	
	}else if (x1 <= x2 && x1 + w1 <= x2) {
			return FALSE;
		// ����Aλ�ھ���B���²�		
	}else if (y1 >= y2 && y1 >= y2 + h2) {
			return FALSE;
		// ����Aλ�ھ���B���ϲ�		
	}else if (y1 <= y2 && y1 + h1 <= y2) {
		return FALSE;
	}
	// ���ཻ�������㣬�Ǿ����ཻ��
	return TRUE;	
}

//����������ε��ص�����
BOOL  _INLINE_ GetOverLapRect(HXRECT r1, HXRECT r2, HXRECT res) {
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
